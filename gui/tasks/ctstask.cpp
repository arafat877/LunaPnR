// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include "ctstask.h"
#include "lunacore.h"

void Tasks::CTS::execute(GUI::Database &database, ProgressCallback callback)
{
    m_status.store(Status::RUNNING);

    auto topModule = database.design().getTopModule();

    if (!topModule)
    {
        error("Top module not set\n");
        return;
    }    

    if (!topModule->m_netlist)
    {
        error("Top module has no netlist\n");
        return;        
    }

    auto netlist = topModule->m_netlist;

    // FIXME: for now use the first region to place the top module
    // check there is a valid floorplan
    if (database.floorplan()->regionCount() == 0)
    {
        error("No regions defined in floorplan!\n");
        return;
    }

    for(auto pin : topModule->m_pins)
    {
        auto pinInstance = netlist->lookupInstance(pin->m_name);
        if (!pinInstance.isValid())
        {
            error(Logging::fmt("Module %s does not have a pin instance corresponding to pin %s!\n", topModule->name().c_str(), pin->m_name.c_str()));
            return;            
        }
        if (!pinInstance->isPlaced())
        {
            error(Logging::fmt("Pin %s of module %s has not been placed!\n", pin->m_name.c_str(), topModule->name().c_str()));
            return;
        }
    }

    info("Running CTS\n");
    //FIXME: temporarily hard-code the clock net and the buffer cell

    auto &design = database.design();

    // insert buffers
    auto bufferCell = design.m_cellLib->lookupCell("CLKBUF2");
    auto bufferInput = bufferCell->lookupPin("A");
    auto bufferOutput = bufferCell->lookupPin("Y");

    auto clkNet = topModule->m_netlist->lookupNet("clk");

    LunaCore::CTS::MeanAndMedianCTS cts;

    auto clockTree = cts.generateTree("clk", *netlist);
    if (!clockTree)
    {
        error("Could not generate clock tree");
        return;
    }

    LunaCore::CTS::MeanAndMedianCTS::CTSInfo ctsinfo;

    ctsinfo.m_pinCapacitance = bufferInput->m_cap;
    ctsinfo.m_inputPinKey    = bufferInput.key();
    ctsinfo.m_outputPinKey   = bufferOutput.key();
    ctsinfo.m_bufferCell     = bufferCell.ptr();
    ctsinfo.m_maxCap         = 0.2e-12;     // 200 fF, no idea if this is realistic
    ctsinfo.m_clkNetKey      = clkNet.key();
    
    LunaCore::CTS::MeanAndMedianCTS::BufferResult bresult;
    bresult = cts.insertBuffers(clockTree.value(), 0, *netlist, ctsinfo);
    std::cout << "  Buffered net has " << bresult.m_totalCapacitance << " F at the input\n";

    // connect clk net to buffer
    for(auto const& sink : bresult.m_list)
    {
        // connect net to instance
        clkNet->addConnection(sink.m_instanceKey, sink.m_pinKey);

        // connect instance to net
        auto connInsKeyPtr = netlist->lookupInstance(sink.m_instanceKey);
        connInsKeyPtr->setPinNet(sink.m_pinKey, clkNet.key());
    }

    clkNet->setClockNet(true);

#if 0
    info("Placing fillers..\n");
    LunaCore::FillerHandler fillerHandler(*cellLib.get());

    fillerHandler.addFillerByName(*database.cellLib().get() ,"FILL");
    if (!fillerHandler.placeFillers(database.design(), *firstRegion, *netlist))
    {
        warning("  failed!\n");
    }
    else
    {
        info("  succeeded!\n");
    }
#endif

    auto regionIter = database.floorplan()->begin();
    if (regionIter == database.floorplan()->end())
    {
        error("No regions defined in floorplan!\n");
        return;
    }
    
    auto firstRegion = *regionIter;
    if (!firstRegion.isValid())
    {
        error("First region in floorplan is invalid!\n");
        return;
    }

    // legalise the cells
    LunaCore::Legalizer cellLegalizer;
    if (!cellLegalizer.legalizeRegion(*firstRegion, *netlist))
    {
        error("  row legalizer failed!");
        return;
    }

    // temporarily save the placement as a DEF file
    std::ofstream deffile("placement_cts.def");
    LunaCore::DEF::WriterOptions options;
    options.exportDecap   = false;
    options.exportFillers = false;
    if (LunaCore::DEF::write(deffile, topModule, options))
    {
        info("  CTS DEF file written.\n");
    }

    info("CTS Done.\n");
    done();
}
