//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_DBE_SCENEWINDOW_H
#define DOGBONE_DBE_SCENEWINDOW_H

#include "DBE_API.h"

#include <IMGUI/IMGUI_Window.h>

#include <GS/GS_CameraController.h>

#include <GR/GR_Framebuffer.h>

#include <glm/glm.hpp>

#include <memory>

namespace dogb
{
    namespace UT
    {
    class Window;
    class Timestep;
    }
}

namespace dogb::DBE
{

class DB_DBE_API SceneWindow : public IMGUI::Window
{
    RTTR_ENABLE(IMGUI::Window)
public:
    explicit SceneWindow();

    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;

    static void menuItem();

    glm::vec2 m_viewportSize;
};

}

#endif // DOGBONE_DBE_SCENEWINDOW_H
