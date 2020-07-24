//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_HIERARCHYWINDOW_H
#define DOGBONE_HIERARCHYWINDOW_H

#include <IMGUI/IMGUI_Window.h>

namespace dogb
{
class HierarchyWindow : public IMGUI::Window
{
    RTTR_ENABLE()

public:
    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;
};
} // namespace dogb

#endif // DOGBONE_HIERARCHYWINDOW_H
