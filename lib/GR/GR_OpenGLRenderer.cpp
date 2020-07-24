//
// Created by matt on 2020-07-11.
//

#include "GR_OpenGLRenderer.h"

#include <CE/CE_GLFWHelpers.h>
#include "GR_VertexArray.h"

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
static bool s_GLADInitialized = false;

GLRenderer::GLRenderer() : RendererAPI(Api::OpenGL) {}

void
GLRenderer::drawIndexed(const std::shared_ptr<VertexArray> vao, uint32_t count)
{
    if (count == 0)
        count = vao->indexBuffer()->count();

    glDrawElements(
            GL_TRIANGLES, static_cast<GLsizei>(count),
            GL_UNSIGNED_INT, nullptr);
}

void
GLRenderer::setClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void
GLRenderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void
gr_RendererUse(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
}

void
GLRenderer::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void
GLRenderer::initWindow(UT::Window* window)
{
    GLFWwindow* glfw_window = reinterpret_cast<GLFWwindow*>(window->nativeWindow());
    gr_RendererUse(glfw_window);

    if (!s_GLADInitialized)
    {
        s_GLADInitialized = true;
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            UT_LOG_ERROR("Failed to initialize GLAD");
            return;
        }
        UT_LOG_INFO("OpenGL Info: ");
        UT_LOG_INFO("  Vendor: {}", glGetString(GL_VENDOR));
        UT_LOG_INFO("  Renderer: {}", glGetString(GL_RENDERER));
        UT_LOG_INFO("  Version: {}", glGetString(GL_VERSION));
    }
}

void
GLRenderer::swapBuffers(UT::Window *window)
{
    GLFWwindow* glfw_window = reinterpret_cast<GLFWwindow*>(window->nativeWindow());
    gr_RendererUse(glfw_window);
    glfwSwapBuffers(glfw_window);
}

void
GLRenderer::use(UT::Window *window)
{
    GLFWwindow *glfw_window = reinterpret_cast<GLFWwindow*>(window->nativeWindow());
    gr_RendererUse(glfw_window);
}
void
GLRenderer::setViewport(I32 x, I32 y, I32 width, I32 height)
{
    glViewport(x, y, width, height);
}

} // namespace dogb::GR::OpenGL