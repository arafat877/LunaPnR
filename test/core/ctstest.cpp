// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "lunacore.h"

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CTSTest)

BOOST_AUTO_TEST_CASE(check_cts)
{
    std::cout << "--== CHECK CTS ==--\n";
        
    std::ifstream leffile("test/files/iit_stdcells/lib/tsmc018/lib/iit018_stdcells.lef");
    BOOST_REQUIRE(leffile.good());

    ChipDB::Design design;
    BOOST_REQUIRE(ChipDB::LEF::Reader::load(design, leffile));

    std::ifstream verilogfile("test/files/verilog/picorv32.v");
    BOOST_REQUIRE(verilogfile.good());

    BOOST_REQUIRE(ChipDB::Verilog::Reader::load(design, verilogfile));
    
    auto mod = design.m_moduleLib->lookupModule("picorv32");
    BOOST_REQUIRE(mod.isValid());

    auto netlist = mod->m_netlist;
    BOOST_REQUIRE(netlist);

    BOOST_CHECK(!LunaCore::CTS::doStuff("clk_doesnt_exist", *netlist));
    BOOST_CHECK(!LunaCore::CTS::doStuff("clk", *netlist));  // fails because cells have not been placed

}

BOOST_AUTO_TEST_SUITE_END()