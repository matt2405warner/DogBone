//
// Created by matt on 2020-07-27.
//

#include "DBE_Editor.h"

#include <GS/GS_Entity.h>

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

    ImGui::Text("ID: %d", entt::to_integral(static_cast<entt::entity>(entity)));

    for (auto &[comp_id, info] : mgr.m_types)
    {
        if (!mgr.hasComponent(entity, comp_id))
            continue;

        ImGui::PushID(static_cast<int>(comp_id));

        if (ImGui::Button("-"))
        {
            info.m_removeCallback(mgr, entity);
            ImGui::PopID();
            continue;
        }
        else
        {
            ImGui::SameLine();
        }

        if (ImGui::CollapsingHeader(info.m_name.c_str()))
        {
            ImGui::Indent(30.0f);

            ImGui::PushID("Widget");
            info.m_guiCallback(mgr, entity);
            ImGui::PopID();

            ImGui::Unindent(30.0f);
        }

        ImGui::PopID();
    }

    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent_Popup");
    }

    if (ImGui::BeginPopup("AddComponent_Popup"))
    {
        ImGui::ListBoxHeader("");
        for (auto &&[id, type] : mgr.m_types)
        {
            (void)id;

            ImGui::PushID((int)id);
            // Check if we can add this component.
            bool is_selectable = !mgr.hasComponent(entity, id);
            ImGuiSelectableFlags_ flags = is_selectable ?
                                                  ImGuiSelectableFlags_None :
                                                  ImGuiSelectableFlags_Disabled;
            bool selected = false;
            if (ImGui::Selectable(type.m_name.c_str(), &selected, flags))
            {
                // Add the component
                type.m_createCallback(mgr, entity);
            }
            ImGui::PopID();
        }
        ImGui::ListBoxFooter();
        ImGui::EndPopup();
    }

    ImGui::PopID();
}
} // namespace dogb::DBE