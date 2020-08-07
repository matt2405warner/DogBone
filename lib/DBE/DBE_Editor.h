//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_DBE_EDITOR_H
#define DOGBONE_DBE_EDITOR_H

#include "DBE_API.h"

#include <string>
#include <functional>

namespace dogb::GS
{
class Entity;
class EntityManager;
}

namespace dogb::DBE
{
class DB_DBE_API Editor
{
public:
    static void drawEntity(GS::EntityManager& mgr, GS::Entity& entity);
    static void drawMenus();

    static void addMenuCallback(const std::string& name, std::function<void()> clb);

private:

    static Editor& instance();

    struct EditorMenu
    {
        EditorMenu() = default;
        explicit EditorMenu(const std::string& name) :
            m_name(name)
        {}
        EditorMenu(const std::string& name, std::function<void()> clb) :
            m_name(name), m_clb(std::move(clb))
        {}

        std::string m_name;
        std::function<void()> m_clb;

        std::vector<EditorMenu> m_items;
    };

    static void drawMenus_(const std::vector<EditorMenu>& menus);

    std::vector<EditorMenu> m_menus;
};
}

#endif // DOGBONE_DBE_EDITOR_H
