#include <unordered_map>
#include "common/logging.h"
#include "cellplacer.h"
#include "qlaplacer.h"
#include "rowlegalizer.h"

using namespace LunaCore::QLAPlacer;


bool LunaCore::QLAPlacer::place(
    const ChipDB::Region  &region,
    ChipDB::Netlist &netlist,
    std::function<void(const LunaCore::QPlacer::PlacerNetlist &)> callback)
{
    double area = 0.0f;

    const auto regionRect = region.m_rect;

    if (region.m_rows.empty())
    {
        doLog(LOG_ERROR,"Cannot place: region has now rows\n");
        return false;
    }

    doLog(LOG_INFO, "Placing netlist in rectangle (%d,%d)-(%d,%d).\n",
        regionRect.left(), regionRect.bottom(), 
        regionRect.right(), regionRect.top());

    // check if pins have been fixed
    // and total the area
    for(auto ins : netlist.m_instances)
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

    const auto regionArea = regionRect.width() * regionRect.height();

    if (regionArea < area)
    {
        std::stringstream ss;
        ss << "The region area (" << regionArea << ") is smaller than the total instance area (" << area << ")\n";
        doLog(LOG_ERROR, ss);
        return false;
    }

    doLog(LOG_INFO, "Utilization = %3.1f percent\n", 100.0* area / static_cast<double>(regionArea));

    auto placerNetlist = Private::createPlacerNetlist(netlist);
    
    Private::doInitialPlacement(regionRect, placerNetlist);    
    Private::updatePositions(placerNetlist, netlist);

    for(uint32_t i=0; i<5; i++)
    {
        Private::doQuadraticB2B(placerNetlist);
        Private::updatePositions(placerNetlist, netlist);
        //Private::lookaheadLegaliser(regionRect, placerNetlist);
        
        if (callback)
        {
            callback(placerNetlist);
        }

        doLog(LOG_INFO,"Iteration %d HPWL %f\n", i, Private::calcHPWL(placerNetlist));
    }

    LunaCore::Legalizer::legalizeRegion(region, netlist, 800);

    doLog(LOG_INFO, "Placement done.\n");

    return true;
}
