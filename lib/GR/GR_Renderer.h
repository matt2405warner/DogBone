//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_RENDERER_H
#define DOGBONE_GR_RENDERER_H

#include "GR_API.h"

#include <SYS/SYS_Types.h>

#include <glm/glm.hpp>

#include <memory>
#include <utility>

namespace dogb::UT
{
class Window;
}

namespace dogb::GR
{
class VertexArray;
class Camera;
class Shader;

enum class Api
{
    None = 0,
    OpenGL
};

namespace detail
{
class DB_GR_API RendererAPI
{
public:

    explicit RendererAPI(Api api) : m_api(api) {}
    virtual ~RendererAPI() = default;

    [[nodiscard]] inline Api api() const { return m_api; }
    virtual void init() = 0;
    virtual void drawIndexed(std::shared_ptr<VertexArray> vao, uint32_t count = 0) = 0;
    virtual void setClearColor(const glm::vec4 &color) = 0;
    virtual void clear() = 0;
    virtual void swapBuffers(UT::Window *window) = 0;
    virtual void use(UT::Window *window) = 0;
    virtual void initWindow(UT::Window *window) = 0;
    virtual void setViewport(I32 x, I32 y, I32 width, I32 height) = 0;

private:
    Api m_api = Api::None;
};
}

class DB_GR_API Renderer
{
public:
    static inline Api api() { return theApi->api(); }

    static void init() { theApi->init(); }
    static void beginScene(Camera &camera, const glm::mat4& transform);
    static void endScene();
    static void drawIndexed(std::shared_ptr<VertexArray> vao, uint32_t count = 0)
    {
        theApi->drawIndexed(std::move(vao), count);
    }
    static void submit(Shader *shader, std::shared_ptr<VertexArray> vao, const glm::mat4 &transform = glm::mat4(1.0f));
    static inline void setClearColor(const glm::vec4 &color)
    {
        theApi->setClearColor(color);
    }
    static inline void clear() { theApi->clear(); }
    static inline void swapBuffers(UT::Window *window) { theApi->swapBuffers(window); }
    static inline void use(UT::Window *window) { theApi->use(window); }
    static inline void initWindow(UT::Window *window) { theApi->initWindow(window); }
    static inline void setViewport(I32 x, I32 y, I32 width, I32 height) { theApi->setViewport(x, y, width, height); }
private:
    struct SceneContext
    {
        glm::mat4 m_viewProjMatrix;
    };
    static SceneContext theSceneCtx;

    static std::unique_ptr<detail::RendererAPI> theApi;
};
} // namespace dogb::GR

#endif // DOGBONE_GR_RENDERER_H
