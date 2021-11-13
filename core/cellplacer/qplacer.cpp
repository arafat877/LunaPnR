#include <cassert>
#include "qplacer.h"

#include "common/logging.h"
#include "design/design.h"
#include "densitybitmap.h"

using namespace LunaCore::QPlacer;

void Placer::checkAndSetUnmovableNet(PlacerNet &net, const std::vector<PlacerNode> &nodes)
{
    // check that at least one node is movable
    ssize_t movableNodes = 0;
    for(auto const netNodeId : net.m_nodes)
    {
        if (!nodes.at(netNodeId).isFixed())
        {
            movableNodes++;
            break;
        }
    }

    if (movableNodes == 0)
    {
        // ignore the net when no nodes are movable
        net.m_type = LunaCore::QPlacer::PlacerNetType::Ignore;
    }
}

void Placer::addStarNodes(PlacerNetlist &netlist)
{
    auto &nets  = netlist.m_nets;
    auto &nodes = netlist.m_nodes;

    // add a star node to every net with more than 2 nodes
    ssize_t netId = 0;
    for(auto& net : nets)
    {
        checkAndSetUnmovableNet(net, nodes);
        
        if (net.m_type != LunaCore::QPlacer::PlacerNetType::Ignore)
        {
            if (net.m_nodes.size() > 2)
            {
                // create a star node
                nodes.emplace_back();
                auto &starNode  = nodes.back();
                starNode.m_type = PlacerNodeType::StarNode;
                starNode.m_connections.push_back(netId);

                // add the star node to the net
                // it should always be the last node
                PlacerNodeId starNodeId = nodes.size()-1;
                net.m_nodes.push_back(starNodeId);

                net.m_type = LunaCore::QPlacer::PlacerNetType::StarNet;
            }
            else if (net.m_nodes.size() == 2)
            {
                net.m_type = LunaCore::QPlacer::PlacerNetType::TwoNet;
            }
        }
        netId++;
    }
}

void Placer::solve(const PlacerNetlist &netlist,
    Eigen::VectorXd &xpos, Eigen::VectorXd &ypos)
{
    PlacerNetlist localNetlist = netlist;

    // solve the quadratic placement problem
    addStarNodes(localNetlist);

    buildEquations<XAxisAccessor>(localNetlist, m_Amat_x, m_Bvec_x);
    buildEquations<YAxisAccessor>(localNetlist, m_Amat_y, m_Bvec_y);

    const auto numberOfNodes = localNetlist.m_nodes.size();
    xpos.resize(numberOfNodes);
    ypos.resize(numberOfNodes);

    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Upper | Eigen::Lower> solver;
    xpos = solver.compute(m_Amat_x).solve(m_Bvec_x);
    ypos = solver.compute(m_Amat_y).solve(m_Bvec_y);
}

std::string LunaCore::QPlacer::toString(const PlacerNodeType &t)
{
    switch(t)
    {
    case PlacerNodeType::MovableNode:
        return "MovableNode";
    case PlacerNodeType::FixedNode:
        return "FixedNode";
    case PlacerNodeType::StarNode:
        return "StarNode";
    default:
        return "Undefined";
    }
}


std::string toString(const PlacerNetType &t)
{
    switch(t)
    {
    case PlacerNetType::Ignore:
        return "Ignore";
    case PlacerNetType::StarNet:
        return "StarNet";
    case PlacerNetType::TwoNet:
        return "TwoNet";
    
    default:
    case PlacerNetType::Undefined:
        return "Undefined";
    }
}

bool LunaCore::QPlacer::placeModuleInRegion(const ChipDB::Design *design, ChipDB::Module *mod, const ChipDB::Region *region)
{
    if (design == nullptr)
    {
        return false;
    }

    if (mod == nullptr)
    {
        return false;
    }

    if (region == nullptr)
    {
        return false;
    }

    if (!mod->m_netlist)
    {
        return false;
    }

    double area = 0;

    // check if pins have been fixed
    // and total the area
    for(auto ins : mod->m_netlist->m_instances)
    {
        if (ins->m_insType == ChipDB::InstanceType::PIN)
        {
            if (ins->m_placementInfo != ChipDB::PlacementInfo::PLACEDANDFIXED)
            {
                std::stringstream ss;
                ss << "Not all pins have been placed and fixed - for example: " << ins->m_name << "\n";
                doLog(LOG_ERROR, ss);
                return false;
            }
        }

        area += ins->instanceSize().m_x * ins->instanceSize().m_y;
    }

    const auto regionArea = region->m_rect.width() * region->m_rect.height();

    if (regionArea < area)
    {
        doLog(LOG_WARN, "Region area is smaller than the total instance area\n");
    }
    
    if (regionArea <= 0)
    {
        doLog(LOG_ERROR, "Cannot place a module in a region that has no area\n");
        return false;
    }

    doLog(LOG_INFO, "Utilization = %3.1f percent\n", 100.0* area / static_cast<double>(regionArea));

    // generate QPlacer netlist
    PlacerNetlist placerNetlist;
    
    std::unordered_map<ChipDB::InstanceBase*, LunaCore::QPlacer::PlacerNodeId> ins2nodeId;
    std::unordered_map<ChipDB::Net*, LunaCore::QPlacer::PlacerNetId> net2netId;

    // create placer nodes
    for(auto ins : mod->m_netlist->m_instances)
    {
        auto nodeId = placerNetlist.createNode();
        auto& placerNode = placerNetlist.getNode(nodeId);
        placerNode.m_type = LunaCore::QPlacer::PlacerNodeType::MovableNode;
        placerNode.m_pos  = ChipDB::Coord64{0,0};
        placerNode.m_size = ins->instanceSize();

        if (ins->m_placementInfo == ChipDB::PlacementInfo::PLACEDANDFIXED)
        {
            placerNode.m_type = LunaCore::QPlacer::PlacerNodeType::FixedNode;
            placerNode.m_pos  = ins->m_pos;
            placerNode.m_weight = 10.0; // increase the weight of fixed nodes to pull apart the movable instances
        }

        ins2nodeId[ins] = placerNetlist.numberOfNodes()-1;
    }

    // create placer nets
    for(auto net : mod->m_netlist->m_nets)
    {
        auto placerNetId = placerNetlist.createNet();
        auto& placerNet = placerNetlist.getNet(placerNetId);

        for(auto& conn : net->m_connections)
        {
            auto ins = conn.m_instance;
            auto iter = ins2nodeId.find(ins);

            if (iter == ins2nodeId.end())
            {
                doLog(LOG_ERROR, "Cannot find node\n");
                return false;
            }

            auto placerNodeId = iter->second;
            placerNet.m_nodes.push_back(placerNodeId);
            placerNetlist.getNode(placerNodeId).m_connections.push_back(placerNetId);
        }
    }

    // do quadratic placement
    Eigen::VectorXd xpos;
    Eigen::VectorXd ypos;
    LunaCore::QPlacer::Placer placer;
    placer.solve(placerNetlist, xpos, ypos);

    // write back new instance positions
    ssize_t idx = 0;
    double  totalArea = 0;
    for(auto ins : mod->m_netlist->m_instances)
    {
        if (ins->m_placementInfo != ChipDB::PlacementInfo::PLACEDANDFIXED)
        {
            ins->m_placementInfo = ChipDB::PlacementInfo::PLACED;

            // positions returned from the quadratic placement
            // are cell centers, not lower-left.

            const ChipDB::Coord64 llpos = {static_cast<int64_t>(xpos[idx] - ins->instanceSize().m_x/2), 
                static_cast<int64_t>(ypos[idx] - ins->instanceSize().m_y/2)};
            ins->m_pos = llpos;
        }

        totalArea += ins->getArea();

        idx++;
    }

    doLog(LOG_VERBOSE,"Quadratic placement done.\n");

#if 0    
    doLog(LOG_VERBOSE,"Starting diffusion..\n");

    float maxDensity      = 2.0f;
    const auto bitmapCellWidth  = 50000;
    const auto bitmapCellHeight = 50000;
    const float targetDensity   = 0.95;

    std::unique_ptr<DensityBitmap> densityBitmap(createDensityBitmap(mod->m_netlist.get(), region, 
        bitmapCellWidth, bitmapCellHeight));

    //setMinimalDensities(densityBitmap.get(), targetDensity);

    std::unique_ptr<VelocityBitmap> velocityBitmap(new VelocityBitmap(densityBitmap->width(), densityBitmap->height()));

    size_t iterationCount = 0;
    const size_t maxIter = 5000;
    while(maxDensity > targetDensity)
    {

        densityBitmap.reset(createDensityBitmap(mod->m_netlist.get(), region, 
            bitmapCellWidth, bitmapCellHeight));

        setMinimalDensities(densityBitmap.get(), targetDensity);

        calcVelocityBitmap(densityBitmap.get(), velocityBitmap.get());
        updateMovableInstances(mod->m_netlist.get(), region, velocityBitmap.get(), 
            bitmapCellWidth, bitmapCellHeight);

        maxDensity = updateDensityBitmap(densityBitmap.get());

        iterationCount++;
        if (iterationCount >= maxIter)
        {
            break;
        }
        
        doLog(LOG_VERBOSE,"  Iter: %d\t density:%f\n", iterationCount, maxDensity);
    }

#if 0
    calcVelocityBitmap(densityBitmap.get(), velocityBitmap.get());
    updateMovableInstances(mod->m_netlist.get(), region, velocityBitmap.get(), 
        bitmapCellWidth, bitmapCellHeight);
#endif
    doLog(LOG_VERBOSE,"Diffusion done (iterations = %ld).\n", iterationCount);

#endif    
    return true;
}

#if 0
double Placer::calcTotalMass(const std::vector<PlacerNode> &nodes) const
{
    double totalMass = 0.0;
    std::vector<size_t> perm(nodes.size());
    for(size_t idx=0; idx<nodes.size(); idx++)
    {
        perm[idx] = idx;
        totalMass += nodes[idx].m_size.m_x * nodes[idx].m_size.m_y;
    }

    return totalMass;
}
#endif