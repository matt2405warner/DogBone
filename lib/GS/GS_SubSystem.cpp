//
// Created by matt on 2020-07-14.
//

#include "GS_SubSystem.h"

namespace dogb::GS
{
void
SubSystem::init() {
    m_activeScene = std::make_shared<GS::Scene>();

    for (auto&& ctx : m_contexts)
    {
        ctx->onInit();
    }
}

void
SubSystem::update(const UT::Timestep& timestep)
{
    m_activeScene->update(timestep);

    for (auto &ctx : m_contexts)
    {
        ctx->update(timestep);
    }
}
void
SubSystem::shutdown()
{
    for (auto&& ctx : m_contexts)
        ctx->onShutdown();

    m_contexts.clear();
}
}