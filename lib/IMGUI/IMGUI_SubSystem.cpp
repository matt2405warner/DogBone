//
// Created by matt on 2020-07-11.
//

#include "IMGUI_SubSystem.h"

#include "IMGUI_Editor.h"
#include "IMGUI_GLFWImpl.h"
#include "IMGUI_OpenGLImpl.h"

namespace dogb::IMGUI
{
void
SubSystem::init()
{
    m_currentCtx = nullptr;
}

void
SubSystem::update(const UT::Timestep& timestep)
{
    for (auto &ctx : m_contexts)
    {
        m_currentCtx = ctx.get();
        ctx->update(timestep);
    }
}
void
SubSystem::shutdown()
{
    for (auto&& ctx : m_contexts)
        ctx->onShutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}

Modal*
SubSystem::addModal(std::unique_ptr<Modal> modal)
{
    if (m_currentCtx == nullptr)
        return nullptr;

    return m_currentCtx->addModal(std::move(modal));
}

} // namespace dogb::IMGUI