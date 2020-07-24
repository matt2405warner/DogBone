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

    ImGui::PopID();
}
} // namespace dogb
