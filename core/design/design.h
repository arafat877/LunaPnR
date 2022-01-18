#pragma once

#include <vector>
#include "floorplan/floorplan.h"
#include "netlist/netlist.h"
#include "techlib/techlib.h"
#include "celllib/celllib.h"

namespace ChipDB
{

class Design
{
public:
    Design() : m_topModule(nullptr), m_uniqueIDCounter(0) {}

    /** clear netlist, celllib, modules and technology information */
    void clear();

    Netlist                 m_netlist;
    CellLib                 m_cellLib;
    ModuleLib               m_moduleLib;
    TechLib                 m_techLib;
    Floorplan               m_floorplan;

    auto createUniqueID()
    {
        return m_uniqueIDCounter++;
    }

    bool setTopModule(const std::string &moduleName);
    
    std::shared_ptr<ChipDB::Module> getTopModule()
    {
        return m_topModule;
    }

protected:
    std::shared_ptr<ChipDB::Module> m_topModule;
    uint32_t m_uniqueIDCounter;
};

};
