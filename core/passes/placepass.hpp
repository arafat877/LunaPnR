#pragma once
#include <fstream>
#include <filesystem>
#include "common/logging.h"
#include "pass.hpp"

namespace LunaCore::Passes
{

class PlacePass : public Pass
{
public:
    PlacePass() : Pass("place")
    {
        registerNamedParameter("pads", "", 0, false);
        registerNamedParameter("core", "", 0, false);
        registerNamedParameter("cell", "", 0, false);
    }

    virtual ~PlacePass() = default;

    /** execute a pass given a list of input arguments.
        returns true if succesful, else false.
    */
    [[nodiscard]] bool execute(Database &database) override
    {
        if (m_namedParams.contains("pads"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else if (m_namedParams.contains("core"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else if (m_namedParams.contains("cell"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else
        {
            doLog(Logging::LogType::ERROR, "Missing place type, use -pads, -core or -cell\n");
            return false;
        }

        return true;
    }

    /**
        returns help text for a pass.
    */
    std::string help() const noexcept override
    {
        std::stringstream ss;
        ss << "place - place pads/core/cell\n";
        ss << "  place <place type>\n\n";
        ss << "  Place type options:\n";
        ss << "    -pads    : place the IO pads\n";
        ss << "    -core    : place all core cells\n";
        ss << "    -cell    : place a specific cell at a specified position\n";
        ss << "\n";
        return ss.str();
    }

    /**
        returns a one-line short help text for a pass.
    */
    virtual std::string shortHelp() const noexcept
    {
        return "place pads/core/cell";
    }

    /**
        Initialize a pass. this is called by registerPass()
    */
    bool init() override
    {
        return true;
    }
};


};
