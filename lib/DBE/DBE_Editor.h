//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_DBE_EDITOR_H
#define DOGBONE_DBE_EDITOR_H

#include "DBE_API.h"

#include <functional>
#include <string>

namespace dogb::GS
{
class Entity;
class EntityManager;
} // namespace dogb::GS

namespace dogb::DBE
{
class DB_DBE_API Editor
{
public:
    static void drawMenus();

    static bool addMenuCallback(
            const std::string &name,
            std::function<void()> clb);

private:
    Editor();

    static Editor &instance();

    struct EditorMenu
    {
        EditorMenu() = default;
        explicit EditorMenu(const std::string &name) : m_name(name) {}
        EditorMenu(const std::string &name, std::function<void()> clb)
            : m_name(name), m_clb(std::move(clb))
        {
        }

        std::string m_name;
        std::function<void()> m_clb;

        std::vector<EditorMenu> m_items;
    };

    static void drawMenus_(const std::vector<EditorMenu> &menus);

    std::vector<EditorMenu> m_menus;
};
} // namespace dogb::DBE

/* clang-format off */
#define __DBE_PASTE_MACRO1(x, y) x##y
#define __DBE_PASTE_MACRO(x, y) __DBE_PASTE_MACRO1(x, y)
/* clang-format on */

#define DBE_REGISTER_MENU(name, clb)  \
                bool __DBE_PASTE_MACRO(__autoRegisterMenu, __LINE__) = \
                                            dogb::DBE::Editor::addMenuCallback(name, clb);

#endif // DOGBONE_DBE_EDITOR_H
