//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_DBE_INSPECTOR_H
#define DOGBONE_DBE_INSPECTOR_H

#include <IMGUI/IMGUI_Window.h>

namespace dogb::DBE
{
class Inspector : public IMGUI::Window
{
public:
    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;
};
}

#endif // DOGBONE_DBE_INSPECTOR_H
