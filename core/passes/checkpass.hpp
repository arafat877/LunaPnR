#pragma once
#include <fstream>
#include <filesystem>
#include "common/logging.h"
#include "pass.hpp"

namespace LunaCore::Passes
{

class CheckPass : public Pass
{
public:
    CheckPass() : Pass("check")
    {
        registerNamedParameter("design", "", 0, false);
        registerNamedParameter("timing", "", 0, false);
        registerNamedParameter("drc", "", 0, false);
    }

    virtual ~CheckPass() = default;

    /** execute a pass given a list of input arguments.
        returns true if succesful, else false.
    */
    [[nodiscard]] bool execute(Database &database) override
    {
        if (m_namedParams.contains("design"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else if (m_namedParams.contains("timing"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else if (m_namedParams.contains("drc"))
        {
            doLog(Logging::LogType::ERROR, "Not implemented yet\n");
            return false;
        }
        else
        {
            doLog(Logging::LogType::ERROR, "Missing check type, use -design, -timing or -drc\n");
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
        ss << "check - check design/timing/drc\n";
        ss << "  check <check type>\n\n";
        ss << "  check type options:\n";
        ss << "    -design  : check design hierachy and cell libraries\n";
        ss << "    -timing  : check timing of the netlist\n";
        ss << "    -drc     : check the physical layout for design rule violations\n";
        ss << "\n";
        return ss.str();
    }

    /**
        returns a one-line short help text for a pass.
    */
    virtual std::string shortHelp() const noexcept
    {
        return "check design/timing/drc";
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
