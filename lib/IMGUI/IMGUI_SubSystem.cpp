//
// Created by matt on 2020-07-11.
//

#include "IMGUI_SubSystem.h"

namespace dogb::IMGUI
{

void
SubSystem::update(const UT::Timestep& timestep)
{
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
}

} // namespace dogb::IMGUI