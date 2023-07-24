// SPDX-FileCopyrightText: 2021-2023 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "version.h"
#include "common/logging.h"
#include "txtwriter.h"

bool LunaCore::TXT::write(std::ostream &os, const std::shared_ptr<ChipDB::Netlist> netlist)
{
    if (!netlist)
    {
        Logging::doLog(Logging::LogType::ERROR, "TXT writer: netlist is nullptr\n");
        return false;
    }

    os << "# Routing positions to center of the cells. Generated by LunaPnR " << LUNAVERSIONSTRING << "\n";
    os << "# Format: <number of points> <point1 x> <point1 y> ... <pointN x> <pointN y> <newline>\n";

    for(auto const netKeyPair : netlist->m_nets)
    {
        // write number of points in the net
        os << netKeyPair->numberOfConnections() << " ";

        // write locations of all the terminals
        for(auto netConnect : *netKeyPair)
        {
            auto ins = netlist->lookupInstance(netConnect.m_instanceKey);
            if (!ins->isPlaced())
            {
                Logging::doLog(Logging::LogType::WARNING,"TXT writer: instance %s has no location / is not placed.!\n", ins->name().c_str());
                return false;
            }

            os << ins->m_pos.m_x << " " << ins->m_pos.m_y << "  ";
        }
        os << "\n";
    }

    return true;
}

bool LunaCore::TXT::write(std::ostream &os, const std::shared_ptr<ChipDB::Module> mod)
{
    return LunaCore::TXT::write(os, mod->m_netlist);
}
