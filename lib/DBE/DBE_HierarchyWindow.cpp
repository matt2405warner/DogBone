//
// Created by matt on 2020-07-28.
//

#include "DBE_HierarchyWindow.h"

#include <DBE/DBE_Editor.h>

#include <GS/GS_TagComponent.h>
#include <GS/GS_TransformComponent.h>
#include <GS/GS_World.h>

#include <IMGUI/IMGUI_Std.h>
#include <IMGUI/IMGUI_SubSystem.h>

#include <set>

DBE_REGISTER_MENU("Window/Hierarchy", &dogb::DBE::HierarchyWindow::menuItem)

namespace dogb::DBE
{
void
HierarchyWindow::onStart()
{
    m_title = "Hierarchy";

    m_style.m_padding = ImVec2{0, 0};

    m_dockDirection = DockLeft;
}

void
HierarchyWindow::drawEntity(GS::Entity& entity, bool is_root)
{
    if (!entity)
        return;

    GS::World &world = GS::World::instance();

    GS::TagComponent& tag = entity.getComponent<GS::TagComponent>();

    //ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_CollapsingHeader;
    int flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (world.m_selectedEntity == entity)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (!is_root)
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    GS::TransformComponent& transform = entity.getComponent<GS::TransformComponent>();
    if (transform.m_children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    bool draw_children = ImGui::TreeNodeEx(tag.m_name.c_str(), flags);

    if (ImGui::IsItemClicked())
        world.m_selectedEntity = entity;

    if (draw_children)
    {
        for (auto &&child : transform.m_children)
            drawEntity(child, false /*root*/);
    }

    if (!is_root)
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
}

void
HierarchyWindow::onGUI(const UT::Timestep &)
{
    GS::World &world = GS::World::instance();
    UT_ASSERT(world.m_activeScene);
    GS::EntityManager &mgr = world.m_activeScene->m_entityManager;

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
                mgr.destroy(world.m_selectedEntity);
            }

            ImGui::ListBoxFooter();
        }

        ImGui::EndPopup();
    }

    //ImGui::ListBoxFooter();
}
void
HierarchyWindow::menuItem()
{
    HierarchyWindow* window = IMGUI::SubSystem::instance().getWindow<HierarchyWindow>();
    window->show();
}
} // namespace dogb::DBE
