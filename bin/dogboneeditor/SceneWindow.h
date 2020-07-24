//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_SCENEWINDOW_H
#define DOGBONE_SCENEWINDOW_H

#include <IMGUI/IMGUI_Window.h>

#include <GS/GS_CameraController.h>
#include <GR/GR_Framebuffer.h>

#include <UT/UT_Timestep.h>

#include <glm/glm.hpp>

namespace dogb
{
    namespace UT
    {
    class Window;
    }

class SceneWindow : public IMGUI::Window
{
    RTTR_ENABLE()
public:
    explicit SceneWindow(UT::Window* window);

    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;

    GS::CameraController m_cameraController;
    std::shared_ptr<GR::Framebuffer> m_framebuffer;
    glm::vec2 m_viewportSize;
};
}

#endif // DOGBONE_SCENEWINDOW_H
