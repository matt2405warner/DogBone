//
// Created by matt on 2020-07-16.
//

#include "UT_Profiler.h"

namespace dogb::UT::Profiler
{
Profiler &
Profiler::instance()
{
    static Profiler _instance;
    return _instance;
}
} // namespace dogb::UT::Profiler