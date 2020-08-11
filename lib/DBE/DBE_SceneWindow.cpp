//
// Created by matt on 2020-07-27.
//

#include "DBE_SceneWindow.h"

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_World.h>

DBE_REGISTER_MENU("Window/Scene", &dogb::DBE::SceneWindow::menuItem)

namespace dogb::DBE
{
SceneWindow::SceneWindow() : m_viewportSize({0.0f, 0.0f}) {}

void
SceneWindow::onStart()
{
    m_title = "Scene";

    m_style.m_padding = ImVec2{0, 0};
    m_dockDirection = DockNone;
}

void
SceneWindow::onGUI(const UT::Timestep &)
{
    GS::World &world = GS::World::instance();

    // bool is_focused = ImGui::IsWindowFocused();

    ImVec2 viewport_size = ImGui::GetContentRegionAvail();

    if (m_viewportSize.x != viewport_size.x ||
         m_viewportSize.y != viewport_size.y)
    {
        m_viewportSize = {viewport_size.x, viewport_size.y};
        if (m_viewportSize.x <= 0)
            m_viewportSize.x = 1;
        if (m_viewportSize.y <= 0)
            m_viewportSize.y = 1;

        uint32_t width = static_cast<uint32_t>(m_viewportSize.x);
        uint32_t height = static_cast<uint32_t>(m_viewportSize.y);
        world.mainCamera()->m_activeTexture->resize(width, height);

        auto scene = world.m_activeScene;
        UT_ASSERT(scene);
        scene->onViewportResize(width, height);
    }

    uint32_t tex_id =
            world.mainCamera()->m_activeTexture->colorAttachmentRendererID();

    ImGui::Image(
            reinterpret_cast<void *>(tex_id),
            ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});
}

void
SceneWindow::menuItem()
{
    UT::Engine& engine = UT::Engine::get();

    IMGUI::SubSystem* sys = engine.getOrCreateSubSystem<IMGUI::SubSystem>();
    SceneWindow* window = sys->getWindow<SceneWindow>();
    window->show();
}

} // namespace dogb::DBE