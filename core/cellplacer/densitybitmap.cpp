
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <deque>
#include <cassert>
#include "densitybitmap.h"

using namespace LunaCore::QPlacer;

void LunaCore::QPlacer::writeToPGM(std::ostream &os, const DensityBitmap *bitmap)
{
    os << "P5 " << bitmap->width() << " " << bitmap->height() << " 255" << "\n";

    for(int64_t y=0; y<bitmap->width(); y++)
    {
        for(int64_t x=0; x<bitmap->height(); x++)
        {
            auto pixel = bitmap->at(x,y);

            uint8_t pixelValue = 0;
            if (pixel > 0)
            {
                // 100%   utilization -> 100
                // 10%    utilization -> 0
                // 1000%  utilization -> 200
                float dB = 100.0f*std::log10(pixel) + 100.0f;

                dB = std::min(dB,255.0f);
                dB = std::max(dB,0.0f);

                pixelValue = static_cast<uint8_t>(dB);
            }

            static_assert(sizeof(pixelValue)==1, "pixels must be 8 bits wide!");

            os.write((char*)&pixelValue, sizeof(pixelValue));
        }
    }
}

DensityBitmap* LunaCore::QPlacer::createDensityBitmap(const ChipDB::Netlist *netlist, const ChipDB::Region *region,
    int64_t bitmapCellWidth /* nm */, int64_t bitmapCellHeight /* nm */)
{
    // The region is covered by the bitmap and divides into bitmap cells
    // Determine the bitmap size:

    auto regionSize   = region->m_rect.getSize();
    auto regionOffset = region->m_rect.getLL();

    // -1 is needed to make sure there isn't an off-by-one error when
    // regionSize is exactly divisible by bitmapCellXXX
    auto xcells = static_cast<ssize_t>(1+std::floor((regionSize.m_x-1) / static_cast<double>(bitmapCellWidth)));
    auto ycells = static_cast<ssize_t>(1+std::floor((regionSize.m_y-1) / static_cast<double>(bitmapCellHeight)));

    std::unique_ptr<DensityBitmap> bitmap(new DensityBitmap(xcells, ycells));

    // sweep from left to right
    // queue: instances that will become active.
    // active: instances that are active.

    // make local copy of instance pointers
    std::deque<ChipDB::InstanceBase*> queue(netlist->m_instances.begin(), netlist->m_instances.end());

    // remove all nullptr instances
    auto removeIter  = std::remove_if(queue.begin(), queue.end(), [](auto ptr){ return ptr==nullptr; } );
    queue.erase(removeIter, queue.end());

    // sort the instances according to x position
    // least x first
    std::sort(queue.begin(), queue.end(),
        [&](auto const ins1, auto const ins2)
        {
            return ins1->m_pos.m_x  < ins2->m_pos.m_x;
        }
    );

    // add one fake end cell to the queue
    // to make sure the entire area is processed
    auto terminationInstance = new TerminationInstance();
    terminationInstance->m_pos = {regionSize.m_x+1, regionSize.m_y/2};
    queue.push_back(terminationInstance);

    std::deque<ChipDB::InstanceBase*> active;

    int64_t x = 0;  // sweep position    
    int64_t nextGridx = bitmapCellWidth;
    int64_t gridxIndex = 0;

    while(!queue.empty())
    {
        int64_t newx = queue.front()->m_pos.m_x - regionOffset.m_x;

        if (newx > nextGridx)
        {
            // new grid edge comes before the remaining
            // instances
            newx = nextGridx;
        }

        auto activeIter = active.begin();
        while(activeIter != active.end())
        {
            auto& activeIns = *activeIter;

            auto insSize   = activeIns->instanceSize();
            auto rightEdge = activeIns->m_pos.m_x+insSize.m_x - regionOffset.m_x;

            if (rightEdge < newx)
            {
                // chop the instance into blocks according to the
                // grid cell height

                int64_t gridyIndex = (activeIns->m_pos.m_y - regionOffset.m_y) / bitmapCellHeight;
                int64_t gridyStart = gridyIndex*bitmapCellHeight;

                auto insTopEdge = activeIns->m_pos.m_y + insSize.m_y - regionOffset.m_y;

                for(int64_t gridy=gridyStart; gridy < insTopEdge; gridy += bitmapCellHeight)
                {
                    auto ystart = std::max(gridy, activeIns->m_pos.m_y - regionOffset.m_y);
                    auto ystop  = std::min(gridy + bitmapCellHeight, insTopEdge);

                    auto insCellArea = (ystop-ystart)*(rightEdge-x);

                    bitmap->at(gridxIndex, gridyIndex) += static_cast<double>(insCellArea) / static_cast<double>(bitmapCellHeight*bitmapCellWidth);

                    gridyIndex++;
                }

                // no longer active -> remove it from the list
                activeIter = active.erase(activeIter);
            }
            else
            {                
                // chop the instance into blocks according to the
                // grid cell height

                int64_t gridyIndex = (activeIns->m_pos.m_y - regionOffset.m_y) / bitmapCellHeight;
                int64_t gridyStart = gridyIndex*bitmapCellHeight;

                auto insTopEdge = activeIns->m_pos.m_y + insSize.m_y - regionOffset.m_y;

                for(int64_t gridy=gridyStart; gridy < insTopEdge; gridy += bitmapCellHeight)
                {
                    auto ystart = std::max(gridy, activeIns->m_pos.m_y - regionOffset.m_y);
                    auto ystop  = std::min(gridy + bitmapCellHeight, insTopEdge);

                    auto insCellArea = (ystop-ystart)*(newx - x);

                    bitmap->at(gridxIndex, gridyIndex) += static_cast<double>(insCellArea) / static_cast<double>(bitmapCellHeight*bitmapCellWidth);

                    gridyIndex++;
                }

                activeIter++;
            }
        }   

        // add newly active cells to the active list
        while(!queue.empty() && ((queue.front()->m_pos.m_x - regionOffset.m_x) <= newx))
        {
            active.push_back(queue.front());
            queue.pop_front();
        }

        if (newx == nextGridx)
        {
            nextGridx += bitmapCellWidth;
            gridxIndex++;
        }

        x = newx;             
    }

    return bitmap.release();
}

void LunaCore::QPlacer::calcVelocityBitmap(const DensityBitmap *bm, VelocityBitmap *vm)
{
    assert(vm != nullptr);
    assert(bm != nullptr);

    assert(vm->size() == bm->size());

    const auto w = vm->width();
    const auto h = vm->height();

    for(int64_t y=0; y<h; y++)
    {
        for(int64_t x=0; x<w; x++)
        {
            const float left   = bm->at(x-1,y);
            const float right  = bm->at(x+1,y);
            const float up     = bm->at(x,y+1);
            const float down   = bm->at(x,y-1);
            const float center = bm->at(x,y);

            if (center > 0.01)
            {
                float horizontalVelo = (left-right) / (2.0*center);
                float verticalVelo   = (down-up) / (2.0*center);

                if ((x == 0) || (y ==0) || (x == (w-1)) || (y == (h-1)))
                {
                    vm->at(x,y) = {0,0};
                }
                else
                {
                    vm->at(x,y) = {horizontalVelo, verticalVelo};
                }
            }
            else
            {
                vm->at(x,y) = {0,0};
            }
        }
    }    
}

void LunaCore::QPlacer::updateMovableInstances(ChipDB::Netlist *netlist, const ChipDB::Region *region, 
    VelocityBitmap *vm,
    int64_t bitmapCellWidth, 
    int64_t bitmapCellHeight)
{
    assert(netlist != nullptr);
    assert(region != nullptr);
    assert(vm != nullptr);

    auto regionOffset = region->m_rect.getLL();

    for(const auto ins : netlist->m_instances)
    {
        if (ins == nullptr)
        {
            continue;
        }

        if (ins->m_placementInfo != ChipDB::PlacementInfo::PLACED)
        {
            continue;
        }

        // determine the closest bin center
        const float insGridx = -0.5f + (ins->m_pos.m_x - regionOffset.m_x)/static_cast<double>(bitmapCellWidth);
        const float insGridy = -0.5f + (ins->m_pos.m_y - regionOffset.m_y)/static_cast<double>(bitmapCellHeight);
        auto x = static_cast<ssize_t>(std::floor(insGridx));
        auto y = static_cast<ssize_t>(std::floor(insGridy));

        const float xfrac = insGridx - x;
        const float yfrac = insGridy - y;

        auto vcell    = vm->at(x,y);
        auto vright   = vm->at(x+1,y);
        auto vup      = vm->at(x,y+1);
        auto vupright = vm->at(x+1,y+1);

        // linear interpolation of velocity bitmap
        auto v = vcell + xfrac*(vright-vcell) + yfrac*(vup-vcell)
            + xfrac*yfrac*(vcell+vupright-vup-vright);

        const float deltaT = 0.05;

        ins->m_pos.m_x += v.m_dx*deltaT * static_cast<float>(bitmapCellWidth);
        ins->m_pos.m_y += v.m_dy*deltaT * static_cast<float>(bitmapCellHeight);
    }
}

float LunaCore::QPlacer::updateDensityBitmap(DensityBitmap *bm)
{
    assert(bm != nullptr);

    auto bitmap = *bm;  // make a deep copy

    const float deltaT = 0.05;

    float maxDensity = 0;
    for(int64_t y=0; y<bitmap.height(); y++)
    {
        for(int64_t x=0; x<bitmap.width(); x++)
        {
            const float left   = bitmap.at(x-1,y);
            const float right  = bitmap.at(x+1,y);
            const float up     = bitmap.at(x,y+1);
            const float down   = bitmap.at(x,y-1);
            const float center = bitmap.at(x,y);

            const float hDelta = left + right - 2.0f*center;

            const float vDelta = up + down - 2.0f*center;

            float newDensity = (deltaT)*(hDelta + vDelta);
            
            bm->at(x,y) += newDensity;

            maxDensity = std::max(maxDensity, bm->at(x,y));
        }
    }

    return maxDensity;
}

LunaCore::QPlacer::Velocity LunaCore::QPlacer::operator*(const float &lhs, const LunaCore::QPlacer::Velocity &rhs)
{
    return Velocity{lhs*rhs.m_dx, lhs*rhs.m_dy};
}

bool LunaCore::QPlacer::operator==(const Velocity &lhs, const Velocity &rhs)
{
    return ((lhs.m_dx == rhs.m_dx) && (lhs.m_dy == rhs.m_dy));
}

void LunaCore::QPlacer::setMinimalDensities(DensityBitmap *bm)
{
    size_t overArea  = 0;
    size_t underArea = 0;

    const float maxDensity = 1.0f;

    for(uint32_t y=0; y<bm->height(); y++)
    {
        for(uint32_t x=0; x<bm->width(); x++)
        {
            if (bm->at(x,y) < maxDensity)
            {
                underArea++;
            }
            else
            {
                overArea++;
            }
        }
    }

    if (underArea == 0)
    {
        // no cells to ajust.
        return;
    }

    const float ratio = static_cast<double>(overArea) / static_cast<double>(underArea);

    for(uint32_t y=0; y<bm->height(); y++)
    {
        for(uint32_t x=0; x<bm->width(); x++)
        {
            const auto density = bm->at(x,y);
            if (density < maxDensity)
            {
                float minDensity = maxDensity - (maxDensity - density)*ratio;
                bm->at(x,y) = minDensity;
            }
        }
    }
}
