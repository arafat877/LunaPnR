#include <algorithm>
#include "pin.h"

using namespace ChipDB;

std::string ChipDB::toString(const IOType &iotype)
{
    switch(iotype)
    {
    case IOType::UNKNOWN:
        return std::string("UNKNOWN");
    case IOType::INPUT:
        return std::string("INPUT");
    case IOType::OUTPUT:
        return std::string("OUTPUT");
    case IOType::OUTPUT_TRI:
        return std::string("TRI-STATE");
    case IOType::IO:
        return std::string("INOUT");
    case IOType::POWER:
        return std::string("POWER");
    case IOType::GROUND:
        return std::string("GROUND");
    default:
        return std::string("?");
    }
};


PinInfo* PinInfoList::createPin(const std::string &name)
{    
    auto iter = find(name);
    if (iter == m_pins.end())
    {
        m_pins.push_back(new PinInfo(name));
        return m_pins.back();
    }
    else
    {
        return *iter;
    }
}

PinInfo* PinInfoList::lookup(const std::string &name)
{
    auto iter = find(name);
    if (iter == m_pins.end())
    {
        return nullptr;
    }
    else
    {
        return *iter;
    }
}

const PinInfo* PinInfoList::lookup(const std::string &name) const
{
    auto iter = find(name);
    if (iter == m_pins.end())
    {
        return nullptr;
    }
    else
    {
        return *iter;
    }
}

ssize_t PinInfoList::lookupIndex(const std::string &name) const
{
    size_t idx = 0;
    for(auto *p : m_pins)
    {
        if (p->m_name == name)
        {
            return idx;
        }
        idx++;
    }
    return -1;
}

PinInfoList::ContainerType::iterator PinInfoList::find(const std::string &name)
{
    auto iter = std::find_if(m_pins.begin(), m_pins.end(), 
        [name](auto * p)
        {
            return p->m_name == name;
        }
    );

    return iter;
}

PinInfoList::ContainerType::const_iterator PinInfoList::find(const std::string &name) const
{
    auto iter = std::find_if(m_pins.begin(), m_pins.end(), 
        [name](auto * p)
        {
            return p->m_name == name;
        }
    );

    return iter;
}
