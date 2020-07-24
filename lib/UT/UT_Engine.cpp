//
// Created by matt on 2020-07-11.
//

#include "UT_Engine.h"

namespace dogb::UT
{
Engine &
Engine::get()
{
    static Engine _instance;
    return _instance;
}

} // namespace dogb::UT