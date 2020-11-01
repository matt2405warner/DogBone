//
// Created by matt on 2020-07-27.
//

#include "DBE_Inspector.h"

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_SubSystem.h>
#include <IMGUI/IMGUI_Std.h>

#include <GS/GS_World.h>
#include <GS/GS_Entity.h>
#include <GS/GS_TagComponent.h>
#include <GS/GS_TransformComponent.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

DBE_REGISTER_MENU("Window/Inspector", &dogb::DBE::Inspector::menuItem)

namespace dogb::DBE
{
void
Inspector::onStart()
{
    m_title = "Inspector";

    m_dockDirection = DockRight;
}

void
Inspector::onGUI(const UT::Timestep &)
{
    UT::Window *window = imguiContext()->window();
    if (window == nullptr)
    {
        UT_LOG_ERROR("IMGUI context is not attached to a window.");
        return;
    }

    GS::World &world = GS::World::instance();
    UT_ASSERT(world.m_activeScene);

    GS::EntityManager& mgr = world.m_activeScene->m_entityManager;
    GS::Entity& entity = world.m_selectedEntity;

    if (!mgr.isValid(entity))
        return;

    auto &tag = entity.getComponent<GS::TagComponent>();
    tag.m_name.reserve(100);
    IMGUI::InputText("##Name", tag.m_name);

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent_Popup");
    }

    if (ImGui::BeginPopup("AddComponent_Popup"))
    {
        for (auto &&[id, info] : mgr.m_types)
        {
            // Check if we can add this component.
            bool is_selectable = !mgr.hasComponent(entity, id);
            if (!is_selectable)
                continue;

            if (ImGui::MenuItem(info.m_name.c_str()))
            {
                info.m_createCallback(mgr, entity);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    // Always draw the Transform component first
    GS::DrawComponent<GS::TransformComponent>(mgr, entity);

    for (auto &[comp_id, info] : mgr.m_types)
    {
        if (!mgr.hasComponent(entity, comp_id))
            continue;

        // The transform component is handed by itself.
        if (info.m_type == rttr::type::get<GS::TransformComponent>())
            continue;

        if (info.m_type.is_valid())
        {
            // Check if we want to hide the component.
            auto hide = info.m_type.get_metadata("GUI_HIDE");
            if (hide.is_valid())
            {
                UT_ASSERT_MSG(
                        hide.can_convert<bool>(),
                        "GUI_HIDE is a bool property");
                bool should_hide = hide.convert<bool>();
                if (should_hide)
                    continue;
            }
        }

        info.m_guiCallback(mgr, entity);
    }
}
void
Inspector::menuItem()
{
    Inspector* window = IMGUI::SubSystem::instance().getWindow<Inspector>();
    window->show();
}
} // namespace dogb::DBE