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
Editor &
Editor::instance()
{
    static Editor _editor;
    return _editor;
}

void
Editor::drawEntity(GS::EntityManager &mgr, GS::Entity &entity)
{
    if (!mgr.isValid(entity))
        return;

    auto e_id = entt::to_integral(static_cast<GS::Entity::IdType>(entity));

    ImGui::PushID(static_cast<int>(e_id));

    GS::TagComponent &tag = entity.getComponent<GS::TagComponent>();

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

void
Editor::drawMenus()
{
    auto &_menus = instance().m_menus;

    ImGui::BeginMainMenuBar();
    drawMenus_(_menus);
    ImGui::EndMainMenuBar();
}

void
Editor::drawMenus_(const std::vector<Editor::EditorMenu> &menus)
{
    for (auto &&item : menus)
    {
        // If this is a leaf we just want to draw a menu item.
        if (item.m_items.empty())
        {
            if (item.m_clb && ImGui::MenuItem(item.m_name.c_str()))
            {
                item.m_clb();
            }
        }
        // If this is not a leaf then draw the rest of the items.
        else if (ImGui::BeginMenu(item.m_name.c_str()))
        {
            drawMenus_(item.m_items);

            ImGui::EndMenu();
        }
    }
}

bool
Editor::addMenuCallback(const std::string &path, std::function<void()> clb)
{
    std::size_t idx = path.find_first_of('/');
    std::size_t offset = 0;

    // Split the name
    std::vector<std::string> elements;
    while (idx != std::string::npos)
    {
        //std::size_t next = path.find_first_of('/', idx + 1);
        std::string part(path, offset, idx - offset);
        elements.emplace_back(part);

        //idx = next;
        offset = idx + 1;
        idx = path.find_first_of('/', offset);
    }
    elements.emplace_back(path, offset);

    // There should be a minimum of 2 elements
    if (elements.size() < 2)
        return false;

    auto& _menus = instance().m_menus;

    std::size_t elem_offset = 0;
    if (_menus.empty())
    {
        // Add the first element and then add in the rest of the elements
        _menus.emplace_back(elements[0]);
        elem_offset = 1;
    }

    EditorMenu* current_menu = &_menus[0];
    for (std::size_t i = elem_offset; i < elements.size(); ++i)
    {
        auto &el = elements[i];

        // Check if we are at the end of the elements
        if (i == elements.size() - 1)
        {
            for (auto &menu : current_menu->m_items)
            {
                if (menu.m_name == el)
                    return false;
            }

            current_menu->m_items.emplace_back(el, clb);
            return true;
        }

        bool found = false;
        for (auto &menu : current_menu->m_items)
        {
            if (menu.m_name == el)
            {
                current_menu = &menu;
                found = true;
            }
        }

        // The simpler method would be to treat this like a tree but then the
        // root element is just a wasted element so instead we make it a little
        // harder to follow and skip having a root.
        if (!found)
        {
            if (i == 0)
            {
                current_menu = &_menus.emplace_back(el);
            }
            else
            {
                current_menu->m_items.emplace_back(el);
                EditorMenu* tmp = &current_menu->m_items.back();
                current_menu = tmp;
            }
        }
    }

    return true;
}

} // namespace dogb::DBE