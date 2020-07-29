//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_DBE_HIERARCHYWINDOW_H
#define DOGBONE_DBE_HIERARCHYWINDOW_H

#include "DBE_API.h"

#include <IMGUI/IMGUI_Window.h>

namespace dogb::GS
{
class Entity;
}

namespace dogb::DBE
{

class DB_DBE_API HierarchyWindow : public IMGUI::Window
{
public:
    void onStart() override ;
    void onGUI(const UT::Timestep& ts);

private:
    void drawEntity(GS::Entity& entity, bool is_root);
};

}

#endif // DOGBONE_DBE_HIERARCHYWINDOW_H
