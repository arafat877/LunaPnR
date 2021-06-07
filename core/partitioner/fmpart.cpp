#include <algorithm>
#include "fmpart.h"

#include "netlist/instance.h"

using namespace LunaCore;

int64_t FMPart::distanceToPartition(const Partition &part, const ChipDB::Coord64 &pos)
{
    auto dx = std::max(part.m_region.m_ll.m_x - pos.m_x, int64_t(0));
         dx = std::max(dx, pos.m_x - part.m_region.m_ur.m_x);

    auto dy = std::max(part.m_region.m_ll.m_y - pos.m_y, int64_t(0));
         dy = std::max(dy, pos.m_y - part.m_region.m_ur.m_y);

    return dx+dy;
}

bool FMPart::init(ChipDB::Netlist *nl)
{
    if (nl == nullptr)
        return false;

    m_nodes.resize(nl->m_instances.size());
    m_nets.resize(nl->m_nets.size());

    // use the flags of the instance and 
    // nets to number each entity uniquely

    ssize_t id = 0;
    for(auto ins : nl->m_instances)
    {
        ins->m_flags = id;

        /* use cell width as the weight, as the height of each cell is the same */
        m_nodes[id].m_weight = ins->instanceSize().m_x;

        // check if the instance has a fixed position.
        // if so, assign the instance/node to the closest
        // partition

        if (ins->m_placementInfo == ChipDB::PlacementInfo::PLACEMENT_PLACEDANDFIXED)
        {
            if (distanceToPartition(m_part1, ins->m_pos) < distanceToPartition(m_part2, ins->m_pos))
            {
                m_nodes[id].m_partitionId = 0;
            }
            else
            {
                m_nodes[id].m_partitionId = 1;
            }
            m_nodes[id].m_locked = true;
        }
        else
        {
            // randomly assign a partition
            // FIXME: do some area / fill balancing
            m_nodes[id].m_locked = false;
            if (std::rand() > (RAND_MAX/2))
            {
                m_nodes[id].m_partitionId = 1;
            }
            else
            {
                m_nodes[id].m_partitionId = 0;
            }
        }

        id++;
    }

    id = 0;
    for(auto net : nl->m_nets)
    {
        net->m_flags = id;
        m_nets[id].m_weight = 1;    // all nets are the same weight
        m_nets[id].m_nodesInPartition[0] = 0;
        m_nets[id].m_nodesInPartition[1] = 0;
        id++;
    }

    // set the connected nets of each node
    // and add the node to each net
    for(auto ins : nl->m_instances)
    {
        for(ssize_t pinIndex=0; pinIndex < ins->getNumberOfPins(); ++pinIndex)
        {
            auto net = ins->getConnectedNet(pinIndex);
            if (net != nullptr)
            {
                const auto netIndex  = net->m_flags;
                const auto nodeIndex = ins->m_flags;

                // FIXME: we should check each node for duplicate nets!
                m_nodes[nodeIndex].m_nets.push_back(netIndex);

                // FIXME: we should check each net for duplicate nodes!
                m_nets[netIndex].m_nodes.push_back(nodeIndex);

                m_nets[netIndex].m_nodesInPartition[m_nodes[nodeIndex].m_partitionId]++;
            }
        }
    }

    // calculate gains per instance
    ssize_t nodeId = 0;
    for(auto& node : m_nodes)
    {
        node.m_gain = 0;

        // don't calculate gain for unmovable nodes.
        if (node.m_locked)
        {
            nodeId++;
            continue;
        }

        uint32_t fromPartitionIndex = 0;
        uint32_t toPartitionIndex   = 0;
        if (node.m_partitionId == 0)
        {
            toPartitionIndex   = 1;            
        }
        else if (node.m_partitionId == 1)
        {
            fromPartitionIndex = 1;
        }
        else
        {
            // error -> m_partitionId not correct init'd
        }

        for(auto netId : node.m_nets)
        {
            auto const& net = m_nets[netId];

            // the net is not critical when it has more than two nodes
            // in each partition. Non-critical nets cannot change the
            // gain of a node.

            if (net.m_nodesInPartition[fromPartitionIndex] == 1)
            {
                // moving the last node in a partition to 
                // the other side will uncut the net.
                node.m_gain++;
            }

            if (net.m_nodesInPartition[toPartitionIndex] == 0)
            {
                // moving a node from an uncut net to
                // another partition will cut the net.
                node.m_gain--;
            }            
        }

        nodeId++;
    }

    return true;
}

