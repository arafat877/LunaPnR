#pragma once
#include "lua.hpp"

#include <string>
#include <QString>

#include "../console/mmconsole.h"
#include "../common/database.h"

namespace GUI
{

class LuaWrapper
{
public:
    LuaWrapper(MMConsole *console, Database &db);
    virtual ~LuaWrapper();

    bool run(const char *txt);
    bool run(const QString &txt);
    bool run(const std::string &txt);

    /** dump the LUA stack to a stream */
    void dumpStack(std::ostream &os);

    /** replacement print function for LUA */
    void print();

protected:
    MMConsole *m_console;
    Database &m_db;    
    lua_State *m_L;
};

};