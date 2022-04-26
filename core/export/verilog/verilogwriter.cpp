/*
  LunaPnR Source Code
  
  SPDX-License-Identifier: GPL-3.0-only
  SPDX-FileCopyrightText: 2022 Niels Moseley <asicsforthemasses@gmail.com>
*/

#include "common/logging.h"

#include "celllib/celllib.h"
#include "netlist/net.h"
#include "netlist/netlist.h"
#include "netlist/instance.h"
#include "lunapnr_version.h"

#include "common/visitor.h"

#include "verilogwriter.h"

using namespace LunaCore::Verilog;

static std::string escapeVerilogName(const std::string &name)
{
    auto iter = name.find_first_of('[');
    if (iter != std::string::npos)
    {
        return "\\" + name + " ";
    }

    return name;
}

bool Writer::write(std::ostream &os, const std::shared_ptr<ChipDB::Module> mod)
{
    if (!mod)
    {
        return false;
    }

    if (!mod->isModule())
    {
        return false;
    }

    os << "/* netlist generated by " << LunaCore::lunaversion << " */ \n";

    if (!writeModuleDefinition(os, mod))
    {
        return false;
    }

    for(auto netPtr : mod->m_netlist->m_nets)
    {
        if (!netPtr->m_isPortNet)
        {
            os << "wire " << escapeVerilogName(netPtr->name()) << ";\n";
        }
        else
        {
            auto const& pinInfo = mod->lookupPin(netPtr->name());
            
            if (!pinInfo.isValid())
            {
                Logging::doLog(Logging::LogType::ERROR,"Port net cannot be resolved to module pin!\n");
                return false;
            }

            switch(pinInfo->m_iotype)
            {
            case ChipDB::IOType::INPUT:
                os << "input ";
                break;
            case ChipDB::IOType::OUTPUT:
                os << "output ";
                break; 
            case ChipDB::IOType::IO:
                os << "inout ";
                break;
            default:
                Logging::doLog(Logging::LogType::ERROR, "Verilog writer: unsupported pin type %s\n", toString(pinInfo->m_iotype).c_str());
                return false;            
            }            
            os << escapeVerilogName(netPtr->name()) << ";\n";
        }
    }

    os << "\n\n";

    if(!writeModuleInstances(os, mod))
    {
        return false;
    }

    os << "endmodule\n\n";

    return true;
}

bool Writer::writeModuleDefinition(std::ostream &os, const std::shared_ptr<ChipDB::Module> mod)
{
    os << "module " << mod->name() << "(\n\t";

    bool firstPort = true;
    for(auto const& pin : mod->m_pins)
    {
        if (!firstPort)
        {
            os << ",\n\t";
        }
        os << escapeVerilogName(pin->name());
        firstPort = false;
    }

    os << "\n);\n\n";
    return true;
}

namespace LunaCore::Verilog
{

    class ExportConstVisitor : public ChipDB::ConstVisitor
    {
    public:
        ExportConstVisitor(std::ostream &os, const ChipDB::Netlist &netlist) : m_netlist(netlist), m_os(os), m_ok(true) {}

        [[nodiscard]] bool isOk() const
        {
            return m_ok;
        }

        void visit(const ChipDB::InstanceBase *instance) override 
        {
            Logging::doLog(Logging::LogType::ERROR,"Verilog writer: cannot write InstanceBase to netlist\n");
            m_ok = false;
        };

        void visit(const ChipDB::Instance *instance) override
        {
            if (instance == nullptr)
            {
                m_ok = false;
                return;
            }

            if (instance->m_insType != ChipDB::InstanceType::CELL)
            {
                m_ok = false;
                Logging::doLog(Logging::LogType::ERROR, "Verilog writer: expected a Cell instance but got %s\n", instance->name().c_str());
                return;
            }

            if (!instance->cell())
            {
                // this is not a regular cell, might it be a __NETCON?
                // which must be translated to an assign statement
                if (instance->name() == "__NETCON")
                {
                    auto inputNetPtr  = m_netlist.m_nets.at(0);
                    auto outputNetPtr = m_netlist.m_nets.at(1);
                    if ((!inputNetPtr) || (!outputNetPtr))
                    {
                        //FIXME: what to do when a __NETCONN does not have both sides connected??
                    }
                    else
                    {
                        m_os << "assign " << escapeVerilogName(outputNetPtr->name()) << "=" << escapeVerilogName(inputNetPtr->name()) << ";\n";
                    }
                }
                else
                {
                    m_ok = false;
                    Logging::doLog(Logging::LogType::ERROR, "Verilog writer: can't write instance %s without a cell name!\n", instance->name().c_str());
                    return;
                }
                
            }
            else
            {
                m_os << instance->getArchetypeName() << " " << instance->name() << " (";
                bool firstPin = true;
                for(ChipDB::PinObjectKey pinKey=0; pinKey < instance->getNumberOfPins(); pinKey++)
                {
                    auto const& pin = instance->getPin(pinKey);

                    if (pin.isValid() && (pin.m_netKey != ChipDB::ObjectNotFound))
                    {
                        auto netPtr = m_netlist.m_nets.at(pin.m_netKey);
                        if (!firstPin)
                        {
                            m_os << ",";
                        }

                        m_os << "\n  ." << escapeVerilogName(pin.name()) << "(" << escapeVerilogName(netPtr->name()) << ")";
                        firstPin = false;
                    }
                }

                if (!firstPin)
                {
                    m_os << "\n";
                }

                m_os << ");\n";
            }
        }
        
        void visit(const ChipDB::Cell *cell) override
        {
            Logging::doLog(Logging::LogType::ERROR,"Verilog writer: cannot write Cell to netlist\n");
            m_ok = false;
        }

        void visit(const ChipDB::Module *module) override
        {
            Logging::doLog(Logging::LogType::ERROR,"Verilog writer: cannot write sub-module %s to netlist\n", module->name().c_str());
            m_ok = false;
        }

        void visit(const ChipDB::PinInstance *instance) override {};
        void visit(const ChipDB::PinInfo *pin) override {};
        void visit(const ChipDB::Netlist *nl) override {};
        void visit(const ChipDB::Net *net) override {};
        void visit(const ChipDB::LayerInfo *layer) override {};

    protected:
        const ChipDB::Netlist &m_netlist;
        bool m_ok;
        std::ostream &m_os;
    };

};

bool Writer::writeModuleInstances(std::ostream &os, const std::shared_ptr<ChipDB::Module> mod)
{
    if (!mod)
    {
        return false;
    }

    if (!mod->m_netlist)
    {
        return false;
    }

    ExportConstVisitor v(os, *mod->m_netlist.get());

    for(auto insKeyObjPair : mod->m_netlist->m_instances)
    {
        insKeyObjPair->accept(&v);
        if (!v.isOk())
        {
            return false;
        }
    }

    return true;
}
