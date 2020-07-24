//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_INSPECTOR_H
#define DOGBONE_INSPECTOR_H

#include <IMGUI/IMGUI_Window.h>

namespace dogb
{
class Inspector : public IMGUI::Window
{
    RTTR_ENABLE()

public:
    virtual void onStart() override;
    virtual void onGUI(const UT::Timestep& ts) override;
};

} // namespace dogb

#endif // DOGBONE_INSPECTOR_H
