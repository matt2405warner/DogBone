//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_OPENGLRENDERER_H
#define DOGBONE_GR_OPENGLRENDERER_H

#include "GR_API.h"

#include "GR_Renderer.h"
#include "GR_VertexArray.h"

#include <glm/glm.hpp>

#include <memory>

namespace dogb::GR::OpenGL
{
class DB_GR_API GLRenderer : public detail::RendererAPI
{
public:
    GLRenderer();
    void drawIndexed(std::shared_ptr<VertexArray> vao, uint32_t count) override;
    void setClearColor(const glm::vec4 &color) override;
    void clear() override;
    void swapBuffers(UT::Window* window) override ;
    void use(UT::Window* window) override ;
    void initWindow(UT::Window* window) override;
    void init() override ;
    void setViewport(I32 x, I32 y, I32 width, I32 height) override;
};

} // namespace dogb::GR::OpenGL

#endif // DOGBONE_GR_OPENGLRENDERER_H
