//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_SCENEWINDOW_H
#define DOGBONE_SCENEWINDOW_H

#include <IMGUI/IMGUI_Window.h>

#include <GR/GR_Framebuffer.h>

#include <glm/glm.hpp>

namespace dogb
{
class TestContext_2D;

class SceneWindow : public IMGUI::Window
{
    RTTR_ENABLE()
public:
    explicit SceneWindow(TestContext_2D* ctx) :
        m_ctx(ctx)
    {}

    void onStart() override;
    void onGUI() override;

    std::shared_ptr<GR::Framebuffer> m_framebuffer;
    glm::vec2 m_viewportSize;
    TestContext_2D* m_ctx;
};
}

#endif // DOGBONE_SCENEWINDOW_H
