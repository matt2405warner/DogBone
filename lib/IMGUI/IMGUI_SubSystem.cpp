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
    IMGUI::Editor::initialize();
}

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

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}

} // namespace dogb::IMGUI