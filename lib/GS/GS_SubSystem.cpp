//
// Created by matt on 2020-07-14.
//

#include "GS_SubSystem.h"

#include "GS_World.h"

namespace dogb::GS
{
void
SubSystem::init() {
    World::instance().init();
}

void
SubSystem::update(const UT::Timestep& timestep)
{
    World::instance().update(timestep);
}
void
SubSystem::shutdown()
{
    World::instance().shutdown();
}
SubSystem &
SubSystem::instance()
{
    static SubSystem _instance;
    return _instance;
}
}