// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include "netlist/instance.h"
#include "netlist/netlist.h"

/** Export net terminal positions in a simple text file format 
 *  meant for testing the global router.
 * 
*/
namespace LunaCore::TXT
{
    bool write(std::ostream &os, const std::shared_ptr<ChipDB::Module> module);
    bool write(std::ostream &os, const std::shared_ptr<ChipDB::Netlist> netlist);
};