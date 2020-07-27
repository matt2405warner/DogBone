//
// Created by matt on 2020-07-24.
//

#include "Editor.h"

#include <GS/GS_Entity.h>

#include <UT/UT_Logger.h>

#include <rttr/type>

#include <imgui.h>

#include <unordered_map>

namespace dogb
{
void
EditorGUI(GS::EntityManager &mgr, GS::Entity &e)
{
    if (!mgr.isValid(e))
        return;

    auto e_id = entt::to_integral(static_cast<GS::Entity::IdType>(e));

    ImGui::PushID(static_cast<int>(e_id));

    ImGui::Text("ID: %d", entt::to_integral(static_cast<entt::entity>(e)));

    for (auto &[comp_id, info] : mgr.m_types)
    {
        if (!mgr.hasComponent(e, comp_id))
            continue;

        ImGui::PushID(static_cast<int>(comp_id));

        if (ImGui::Button("-"))
        {
            info.m_removeCallback(mgr, e);
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
            info.m_guiCallback(mgr, e);
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
            bool is_selectable = !mgr.hasComponent(e, id);
            ImGuiSelectableFlags_ flags = is_selectable ?
                                                  ImGuiSelectableFlags_None :
                                                  ImGuiSelectableFlags_Disabled;
            bool selected = false;
            if (ImGui::Selectable(type.m_name.c_str(), &selected, flags))
            {
                // Add the component
                type.m_createCallback(mgr, e);
            }
            ImGui::PopID();
        }
        ImGui::ListBoxFooter();
        ImGui::EndPopup();
    }

    ImGui::PopID();
}
} // namespace dogb
