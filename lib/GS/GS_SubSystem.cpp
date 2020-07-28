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

    m_activeScene = std::make_shared<GS::Scene>();

    for (auto&& ctx : m_contexts)
    {
        ctx->onInit();
    }
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

    for (auto&& ctx : m_contexts)
        ctx->onShutdown();

    m_contexts.clear();
}
}