// SPDX-FileCopyrightText: 2021-2023 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "common/logging.h"
#include "lefreader.h"
#include "lefreaderimpl.h"

using namespace ChipDB::LEF;

bool Reader::load(Design &design, std::istream &source)
{
    try
    {
        std::stringstream src;
        src << source.rdbuf();

        ReaderImpl readerimpl(design);
        if (!readerimpl.parse(src.str()))
        {
            Logging::doLog(Logging::LogType::ERROR,"LEF::Reader failed to load file.\n");
            return false;
        }
        
        return true;
    }
    catch(std::runtime_error &e)
    {
        Logging::doLog(Logging::LogType::ERROR,"%s\n", e.what());
    }
    
    return false;    
}
