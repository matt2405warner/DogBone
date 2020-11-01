//
// Created by matt on 2020-07-27.
//

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_Std.h>

#include <imgui.h>

namespace dogb::DBE
{
Editor::Editor()
{
    // The layout of the menu items should be static across editor startups
    // The current layout is:
    //  File | Entity | Component | Window | Help
    //
    // NB: All other menu items are added after this.
    m_menus.emplace_back("File");
    m_menus.emplace_back("Entity");
    m_menus.emplace_back("Component");
    m_menus.emplace_back("Window");
    m_menus.emplace_back("Help");
}

Editor &
Editor::instance()
{
    static Editor _editor;
    return _editor;
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

    EditorMenu* current_menu = nullptr;

    // Find the top level menu that already exists.
    for (auto&& menu : _menus)
    {
        if (menu.m_name == elements[0])
        {
            current_menu = &menu;
            break;
        }
    }

    // If we did not find the top level menu item then add it in now and set
    // our current menu item to the newly added top level menu item.
    if (current_menu == nullptr)
    {
        // Add the first element and then add in the rest of the elements
        _menus.emplace_back(elements[0]);
        current_menu = &_menus.back();
    }

    for (std::size_t i = 1; i < elements.size(); ++i)
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
            if (i == 0 && !current_menu->m_items.empty())
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