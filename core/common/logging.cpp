/*

    Niels A. Moseley (c) 2020, 2021

*/

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include "logging.h"

static uint32_t gs_loglevel = LOG_WARN;

#if 1
static const char* FGRED     = "\033[38;5;9m";
static const char* FGGREEN   = "\033[38;5;2m";
static const char* FGYELLOW  = "\033[38;5;11m";
static const char* FGNAVY    = "\033[38;5;6m";
static const char* FGWHITE   = "\033[38;5;15m";
static const char* FGDEFAULT = "\033[m";

static const char* FGDEBUG = FGNAVY;
static const char* FGERR   = FGRED;
static const char* FGWARN  = FGYELLOW;
static const char* FGINFO  = FGGREEN;
static const char* FGVERB  = FGWHITE;

#else
static const char* FGDEBUG = "";
static const char* FGERR   = "";
static const char* FGINFO  = "";
static const char* FGWARN  = "";
static const char* FGVERB  = "";
#endif

void setLogLevel(uint32_t level)
{
    gs_loglevel = level;
}


void doLog(uint32_t t, const std::string &txt)
{
    doLog(t, txt.c_str());
}

void doLog(uint32_t t, const char *format, ...)
{   
    if (t < gs_loglevel)
    {
        return;
    }

    std::stringstream ss;
    switch(t)
    {
    case LOG_PRINT:
        break;
    case LOG_INFO:
        ss << FGINFO   << "[INFO] ";
        break;
    case LOG_DEBUG:
        ss << FGDEBUG  << "[DBG ] ";
        break;
    case LOG_WARN: 
        ss << FGWARN   << "[WARN] ";
        break;
    case LOG_ERROR:
        ss << FGERR    << "[ERR ] ";
        break;
    case LOG_VERBOSE:
        ss << FGVERB   << "[VERB] ";
        break;
    default:
        break;
    }

    char buffer[2048];

    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buffer, sizeof(buffer), format, argptr);    
    va_end(argptr);
    ss << buffer << FGDEFAULT;
    std::cout << ss.str();
}

void doLog(uint32_t t, const std::stringstream &txt)
{
    if (t < gs_loglevel)
    {
        return;
    }

    std::stringstream ss;
    switch(t)
    {
    case LOG_PRINT:
        break;
    case LOG_INFO:
        ss << FGINFO   << "[INFO] ";
        break;
    case LOG_DEBUG:
        ss << FGDEBUG  << "[DBG ] ";
        break;
    case LOG_WARN: 
        ss << FGWARN   << "[WARN] ";
        break;
    case LOG_ERROR:
        ss << FGERR    << "[ERR ] ";
        break;
    case LOG_VERBOSE:
        ss << FGVERB   << "[VERB] ";
        break;
    default:
        break;
    }

    std::cout << ss.str() << txt.str() << FGDEFAULT;
}