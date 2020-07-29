//
// Created by matt on 2020-07-28.
//

#include "DBE_HierarchyWindow.h"

#include <GS/GS_TagComponent.h>
#include <GS/GS_TransformComponent.h>
#include <GS/GS_World.h>

#include <IMGUI/IMGUI_Std.h>

#include <set>

namespace dogb::DBE
{
void
HierarchyWindow::onStart()
{
    m_title = "Hierarchy";

    m_style.m_padding = ImVec2{0, 0};
}

void
HierarchyWindow::drawEntity(GS::Entity& entity, bool is_root)
{
    GS::World &world = GS::World::instance();

    GS::TagComponent& tag = entity.getComponent<GS::TagComponent>();

    //ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_CollapsingHeader;
    int flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (world.m_selectedEntity == entity)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (!is_root)
        ImGui::Indent();

    GS::TransformComponent& transform = entity.getComponent<GS::TransformComponent>();
    if (transform.m_children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (ImGui::TreeNodeEx(tag.m_name.c_str(), flags))
    {
        //ImGui::SameLine();
        //IMGUI::InputText("", tag.m_name);



        for (auto&& child : transform.m_children)
            drawEntity(child, false /*root*/);

    }
    if (ImGui::IsItemClicked())
        world.m_selectedEntity = entity;

    if (!is_root)
        ImGui::Unindent();
}

void
HierarchyWindow::onGUI(const UT::Timestep &)
{
    GS::World &world = GS::World::instance();
    GS::EntityManager &mgr = world.m_entityManager;

    std::set<GS::Entity> root_entities;
    mgr.registry().each([&](entt::entity entity) {
      GS::TransformComponent& transform = mgr.registry().get<GS::TransformComponent>(entity);
      root_entities.insert(transform.m_root);
    });

    //ImGui::ListBoxHeader("##", ImVec2(-1, -1));

    for (GS::Entity entity : root_entities)
    {
        drawEntity(entity, true /*root*/);
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::ListBoxHeader("##"))
        {
            if (ImGui::Selectable("Create Entity"))
            {
                GS::Entity e = world.createEntity();
                if (world.m_selectedEntity)
                {
                    world.m_selectedEntity.addChildEntity(e);
                }
                else
                {
                    world.m_selectedEntity = e;
                }
            }
            else if (ImGui::Selectable("Remove Entity"))
            {
                world.m_entityManager.destroy(world.m_selectedEntity);
            }

            ImGui::ListBoxFooter();
        }

        ImGui::EndPopup();
    }

    //ImGui::ListBoxFooter();
}
} // namespace dogb::DBE
