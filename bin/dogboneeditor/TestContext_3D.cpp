//
// Created by matt on 2020-07-14.
//

#include "TestContext_3D.h"

#include <DBE/DBE_SceneWindow.h>

#include <GR/GR_OpenGLShader.h>
#include <GR/GR_RenderBuffer.h>
#include <GR/GR_Renderer.h>
#include <GR/GR_Renderer2D.h>
#include <GR/GR_Texture.h>
#include <GR/GR_VertexArray.h>

#include <UT/UT_Window.h>
#include <UT/UT_Profiler.h>

#include <glm/gtc/matrix_transform.hpp>

namespace dogb
{
TestContext_3D::TestContext_3D(DBE::SceneWindow* scene_window)
    :
    m_color({0.2f, 0.8f, 0.3f, 1.0f})
    , m_sceneWindow(scene_window)
{
}

void
TestContext_3D::onAttach()
{
    UT_PROFILE_FUNCTION();

    m_flatShader = m_shaderLibrary.load("../assets/shaders/FlatShader.glsl");
    m_texShader = m_shaderLibrary.load("../assets/shaders/Texture.glsl");

    /* clang-format off */
    float sqr_vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5,  0.5f,  0.0f, 0.0f, 1.0f
    };
    /* clang-format on */

    m_sqrVAO = GR::VertexArray::create();

    std::shared_ptr<GR::VertexBuffer> sqr_vb(
            GR::VertexBuffer::create(sqr_vertices, sizeof(sqr_vertices)));
    GR::BufferLayout sqr_layout{
            {GR::ShaderDataType::Float3, "a_Position"},
            {GR::ShaderDataType::Float2, "a_TexCoord"}};
    sqr_vb->setLayout(sqr_layout);
    m_sqrVAO->addVertexBuffer(sqr_vb);
    uint32_t sqr_indices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<GR::IndexBuffer> sqr_ibuffer(GR::IndexBuffer::create(
            sqr_indices, sizeof(sqr_indices) / sizeof(uint32_t)));
    m_sqrVAO->setIndexBuffer(sqr_ibuffer);

    m_texture = GR::Texture2D::create("../assets/textures/Checkerboard.png");
    m_texShader->bind();
    m_texShader->setInt("u_Texture", 0);

    m_blendTexture = GR::Texture2D::create("../assets/textures/stones.jpg");
}

void TestContext_3D::update(UT::Timestep)
{
#if 0
    UT_PROFILE_FUNCTION();
    using namespace GR::OpenGL;

    m_framebuffer->bind();

    GR::Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    GR::Renderer::clear();

    GR::Renderer::beginScene(m_sceneWindow->m_cameraController.camera());

    m_flatShader->bind();
    m_flatShader->setFloat4("u_Color", m_color);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            glm::vec3 pos(
                    static_cast<float>(x) * 0.11f,
                    static_cast<float>(y) * 0.11f, 0.0f);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            GR::Renderer::submit(m_flatShader.get(), m_sqrVAO, transform);
        }
    }

    m_texShader->bind();
    m_texture->bind();
    GR::Renderer::submit(
            m_texShader.get(), m_sqrVAO,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.25f)));
    m_blendTexture->bind();
    GR::Renderer::submit(
            m_texShader.get(), m_sqrVAO,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.25f)));
    GR::Renderer::endScene();

    m_framebuffer->unbind();
#endif
}

} // namespace dogb