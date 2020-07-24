//
// Created by matt on 2020-07-14.
//

#include "TestContext_2D.h"

#include "SceneWindow.h"

#include <GR/GR_Renderer.h>
#include <GR/GR_Renderer2D.h>

#include <UT/UT_Profiler.h>

namespace dogb
{
TestContext_2D::TestContext_2D(SceneWindow* scene_window)
    : m_sceneWindow(scene_window)
{
}

void
TestContext_2D::onAttach()
{
    UT_PROFILE_FUNCTION();

    m_texture = GR::Texture2D::create("../assets/textures/Checkerboard.png");
}

void
TestContext_2D::update(UT::Timestep ts)
{
    UT_PROFILE_FUNCTION();

    GR::Renderer::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    GR::Renderer::clear();

    m_framebuffer->bind();

    GR::Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    GR::Renderer::clear();

    GR::Renderer2D::beginScene(m_sceneWindow->m_cameraController.camera());

    GR::Renderer2D::drawQuad(
            {-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    GR::Renderer2D::drawQuad(
            {0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
    GR::Renderer2D::drawQuad({0.2f, 0.5f}, {0.5f, 0.75f}, m_texture, 10.0f);
    GR::Renderer2D::drawQuad({-0.2f, -0.5f}, {0.5f, 0.5f}, m_texture, 2.0f);
    GR::Renderer2D::drawRotatedQuad(
            {-0.8f, -0.8f}, {0.5f, 0.5f}, 45.0f, m_texture, 10.0f,
            {0.2f, 0.8f, 0.3f, 1.0f});
    static float rotation = 0.0f;
    rotation += ts.seconds() * 20.0f;
    GR::Renderer2D::drawRotatedQuad(
            {0.8f, -0.8f}, {0.5f, 0.5f}, rotation, m_texture, 10.0f,
            {0.2f, 0.8f, 0.3f, 1.0f});
   // GR::Renderer2D::endScene();

    //GR::Renderer2D::beginScene(m_cameraController.camera());

    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            glm::vec4 color = {(x + 5) / 10, 0.4f, (y + 5) / 10, 0.5f};
            GR::Renderer2D::drawQuad(
                    {x, y, 1.0f}, {0.45f, 0.45f}, color);
        }
    }

    GR::Renderer2D::endScene();

    m_framebuffer->unbind();
}
} // namespace dogb