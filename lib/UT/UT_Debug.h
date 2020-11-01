//
// Created by matt on 2020-11-01.
//

#ifndef DOGBONE_UT_DEBUG_H
#define DOGBONE_UT_DEBUG_H

#include "UT_API.h"

#include <fmt/core.h>

#include <string>
#include <string_view>
#include <iostream>
#include <experimental/source_location>

namespace dogb::UT
{
    inline void log(std::string_view message, const std::experimental::source_location& sl = std::experimental::source_location::current())
    {
        std::cout << "info: " << sl.file_name() << ":" << sl.line() << ' ' << message << '\n';
    }
}

// Utility to printout a message in any configuration.
#define UTprint(msg) dogb::UT::log(msg);
#define UTformat(_fmt, ...)                                                     \
    {                                                                          \
        std::string str = fmt::format(_fmt, __VA_ARGS__);                       \
        UTprint(str)                                                           \
    }

// Utility to printout a message only in debug.
#if DB_DEBUG
#define UTprintDebug(msg) UTprint(msg)
#define UTformatDebug(fmt, ...) UTformat(fmt, __VA_ARGS__)
#else
#define UTprintDebug(msg)
#define UTformatDebug(fmt, ...)
#endif

#endif // DOGBONE_UT_DEBUG_H
