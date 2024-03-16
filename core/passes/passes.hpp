#pragma once
#include <string>
#include <vector>
#include "database/database.h"

namespace LunaCore::Passes
{
    bool run(Database &database, const std::string &cmdstring);
    void registerAllPasses();

    std::vector<std::string> getNamesOfPasses();
};
