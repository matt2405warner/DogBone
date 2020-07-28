//
// Created by matt on 2020-07-28.
//

#include "DBE_HierarchyWindow.h"

#include <GS/GS_World.h>
#include <GS/GS_TagComponent.h>

namespace dogb::DBE
{
void
HierarchyWindow::onStart()
{
    m_title = "Hierarchy";
}

void
HierarchyWindow::onGUI(const UT::Timestep &)
{
    GS::World& world = GS::World::instance();
    GS::EntityManager& mgr = world.m_entityManager;

    ImGui::ListBoxHeader("##", ImVec2(-1, -1));

    mgr.registry().each([&](entt::entity entity){
        GS::TagComponent& tag = mgr.registry().get<GS::TagComponent>(entity);
        ImGui::PushID((int)entity);
        bool is_selected = static_cast<entt::entity>(world.m_selectedEntity) == entity;
        if (ImGui::Selectable(tag.m_name.c_str(), is_selected))
            world.m_selectedEntity = GS::Entity(entity);
        ImGui::PopID();
    });

    ImGui::ListBoxFooter();
}
}
