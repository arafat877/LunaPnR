// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "common/dbtypes.h"
#include "netlist/netlist.h"

namespace LunaCore
{

/** stupidly simple placed - just places cells in order */
class SimpleCellPlacer
{
public:
    static void place(ChipDB::Netlist *nl, const ChipDB::Rect64 &regionRect, const int64_t rowHeight);
};

};