//
// Created by matt on 2020-07-11.
//

#include "GR_Renderer.h"

#include "GR_VertexArray.h"
#include "GR_Shader.h"
#include "GR_Camera.h"

#include "GR_OpenGLRenderer.h"
#include "GR_OpenGLShader.h"

namespace dogb::GR
{
std::unique_ptr<detail::RendererAPI> Renderer::theApi = std::make_unique<OpenGL::GLRenderer>();
Renderer::SceneContext Renderer::theSceneCtx;

void
Renderer::submit(Shader *shader, std::shared_ptr<VertexArray> vao, const glm::mat4 &transform)
{
    if (!shader || !vao)
        return;

    shader->setMat4("u_ViewProj", theSceneCtx.m_viewProjMatrix);
    shader->setMat4("u_Transform", transform);

    theApi->drawIndexed(vao, vao->indexBuffer()->count());
}

void
Renderer::beginScene(Camera &camera, const glm::mat4& transform)
{
    beginScene(camera.projection() * glm::inverse(transform));
}

void
Renderer::beginScene(const glm::mat4 &view_projection)
{
    theSceneCtx.m_viewProjMatrix = view_projection;
}

void
Renderer::endScene()
{
}
} // namespace dogb::GR