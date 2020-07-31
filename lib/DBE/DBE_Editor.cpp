//
// Created by matt on 2020-07-27.
//

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_Std.h>

#include <GS/GS_Entity.h>
#include <GS/GS_TagComponent.h>

#include <imgui.h>

namespace dogb::DBE
{
void
Editor::drawEntity(GS::EntityManager &mgr, GS::Entity &entity)
{
    if (!mgr.isValid(entity))
        return;

    auto e_id = entt::to_integral(static_cast<GS::Entity::IdType>(entity));

    ImGui::PushID(static_cast<int>(e_id));

    GS::TagComponent& tag = entity.getComponent<GS::TagComponent>();

    tag.m_name.reserve(100);
    IMGUI::InputText("Name", tag.m_name);

    for (auto &[comp_id, info] : mgr.m_types)
    {
        if (!mgr.hasComponent(entity, comp_id))
            continue;

        if (info.m_type.is_valid())
        {
            // Check if we want to hide the component.
            auto hide = info.m_type.get_metadata("GUI_HIDE");
            if (hide.is_valid())
            {
                UT_ASSERT_MSG(hide.can_convert<bool>(), "GUI_HIDE is a bool property");
                bool should_hide = hide.convert<bool>();
                if (should_hide)
                    continue;
            }
        }

        info.m_guiCallback(mgr, entity);
    }

    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent_Popup");
    }

    if (ImGui::BeginPopup("AddComponent_Popup"))
    {
        ImGui::ListBoxHeader("##");
        for (auto &&[id, info] : mgr.m_types)
        {
            ImGui::PushID((int)id);
            // Check if we can add this component.
            bool is_selectable = !mgr.hasComponent(entity, id);
            ImGuiSelectableFlags_ flags = is_selectable ?
                                                  ImGuiSelectableFlags_None :
                                                  ImGuiSelectableFlags_Disabled;
            bool selected = false;
            if (ImGui::Selectable(info.m_name.c_str(), &selected, flags))
            {
                // Add the component
                info.m_createCallback(mgr, entity);
            }
            ImGui::PopID();
        }
        ImGui::ListBoxFooter();
        ImGui::EndPopup();
    }

    ImGui::PopID();
}
} // namespace dogb::DBE