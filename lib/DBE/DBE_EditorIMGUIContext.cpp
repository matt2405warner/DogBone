//
// Created by matt on 2020-07-28.
//

#include "DBE_EditorIMGUIContext.h"

#include <GS/GS_World.h>

#include <imgui.h>

namespace dogb::DBE
{
void
EditorIMGUIContext::onGUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Entity"))
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                GS::World& world = GS::World::instance();
                world.createEntity();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Component"))
        {

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
}
