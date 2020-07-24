//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_UT_ASSERT_H
#define DOGBONE_UT_ASSERT_H

#include "UT_API.h"

#include <fmt/core.h>

#ifdef DB_DEBUG

#if defined(DB_LINUX)
#include <signal.h>
#define UT_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform not currently supported."
#endif

#else

#define DB_DEBUGBREAK()

#endif

#if DB_ASSERT_LEVEL > 0
#define UT_ASSERT_MSG(_cond_, ...)                                             \
    {                                                                          \
        if (!(_cond_))                                                         \
        {                                                                      \
            fmt::print("Assertion Failed: {0}.\n", __VA_ARGS__);               \
            UT_DEBUGBREAK();                                                   \
        }                                                                      \
    }
#define UT_ASSERT(_cond_) UT_ASSERT_MSG(_cond_, #_cond_)

#else
#define UT_ASSERT(_cond_)
#define UT_ASSERT_MSG(_cond_, ...)
#endif

#endif // DOGBONE_UT_ASSERT_H
