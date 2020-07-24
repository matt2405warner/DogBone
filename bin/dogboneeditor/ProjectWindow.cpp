//
// Created by matt on 2020-07-11.
//

#include "ProjectWindow.h"

#include "TestContext_3D.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GR/GR_Renderer2D.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace dogb
{
void
ProjectWindow::onStart()
{
    m_title = "Project";

    m_color = glm::vec4 ( 0.8f, 0.2f, 0.3f , 1.0f);
}

void
ProjectWindow::onGUI()
{
#if 0
    UT::Window *window = imguiContext()->window();
    if (window == nullptr)
    {
        UT_LOG_ERROR("IMGUI context is not attached to a window.");
        return;
    }

    GR::Renderer2D::Statistics stats = GR::Renderer2D::statistics();

    //int win_width = window->width();
    //int win_height = window->height();
    ImGui::Text("Another window...");

    ImGui::Text("Render2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.m_drawCalls);
    ImGui::Text("Quads: %d", stats.m_quadCount);
    ImGui::Text("Vertices: %d", stats.totalVertexCount());
    ImGui::Text("Indices: %d", stats.totalIndexCount());

    if (m_ctx != nullptr)
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_ctx->m_color));
#endif
}

} // namespace rne