//
// Created by matt on 2020-07-27.
//

#include "DBE_SceneWindow.h"

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_World.h>
#include <GS/GS_Entity.h>
#include <GS/GS_TransformComponent.h>
#include <GS/GS_CameraComponent.h>
#include <GS/GS_Math.h>

#include <CE/CE_Input.h>

#include <GR/GR_DesktopWindow.h>

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

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
    if (!world.m_mainCamera)
        return;

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

        world.m_mainCamera->m_activeTexture->resize(width, height);

        auto scene = world.m_activeScene;
        UT_ASSERT(scene);
        scene->onViewportResize(width, height);
    }

    uint32_t tex_id =
            world.m_mainCamera->m_activeTexture->colorAttachmentRendererID();

    ImGui::Image(
            reinterpret_cast<void *>(tex_id),
            ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});

    bool snap = CE::Input::IsKeyPressed(CE::Input::KeyType::KEY_LEFT_CTRL);
    float snap_value = 0.5f;
    if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
        snap_value = 45.0f;

    float snap_values[3] = {snap_value, snap_value, snap_value};

    GS::Entity selected_entity = world.m_selectedEntity;
    if (selected_entity && m_gizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImVec2 pos = ImGui::GetWindowPos();
        auto width = ImGui::GetWindowWidth();
        auto height = ImGui::GetWindowHeight();
        ImGuizmo::SetRect(pos.x, pos.y, width, height);

        auto scene = world.m_activeScene;
        auto entity = scene->getPrimaryCameraEntity();
        auto camera = entity.getComponent<GS::CameraComponent>().m_camera;
        const glm::mat4& projection = camera->projection();
        glm::mat4 camera_view = glm::inverse(entity.getComponent<GS::TransformComponent>().transform());

        auto& tc = selected_entity.getComponent<GS::TransformComponent>();
        glm::mat4 transform = tc.transform();

        ImGuizmo::Manipulate(
                glm::value_ptr(camera_view), glm::value_ptr(projection),
                (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL,
                glm::value_ptr(transform), nullptr,
                snap ? snap_values : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            GS::decomposeTransform(transform, translation, rotation, scale);

            glm::vec3 delta_rotation = rotation - tc.m_rotation;
            tc.m_translation = translation;
            tc.m_rotation += delta_rotation;
            tc.m_scale = scale;
        }
    }
}

void
SceneWindow::menuItem()
{
    SceneWindow* window = IMGUI::SubSystem::instance().getWindow<SceneWindow>();
    window->show();
}
bool
SceneWindow::onUpdateKeys(const CE::Input::KeyEvent &ev)
{
    UT_LOG_INFO("Update key event: {}", ev.m_action)
    using namespace CE::Input;
    if (ev.m_action != ActionType::PRESS)
        return false;

    if (ev.m_key == KeyType::KEY_T)
    {
        m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
        return true;
    }
    else if (ev.m_key == KeyType::KEY_R)
    {
        m_gizmoType = ImGuizmo::OPERATION::ROTATE;
        return true;
    }
    else if (ev.m_key == KeyType::KEY_E)
    {
        m_gizmoType = ImGuizmo::OPERATION::SCALE;
        return true;
    }
    else if (ev.m_key == KeyType::KEY_Q)
    {
        m_gizmoType = -1;
        return true;
    }
    return false;
}

} // namespace dogb::DBE