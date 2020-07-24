//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_PROJECTWINDOW_H
#define DOGBONE_PROJECTWINDOW_H

#include <IMGUI/IMGUI_Window.h>

#include <glm/glm.hpp>

namespace dogb
{
class TestContext_3D;

class ProjectWindow : public IMGUI::Window
{
    RTTR_ENABLE()
public:
    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;

    glm::vec4 m_color;

    TestContext_3D* m_ctx{};
};

} // namespace dogb

#endif // DOGBONE_PROJECTWINDOW_H
