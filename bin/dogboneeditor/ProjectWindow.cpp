//
// Created by matt on 2020-07-11.
//

#include "ProjectWindow.h"

#include "TestContext_3D.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GS/GS_Scene.h>
#include <GS/GS_SubSystem.h>

#include <GR/GR_Renderer2D.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

#include <imgui.h>

namespace dogb
{
void
ProjectWindow::onStart()
{
    m_title = "Project";

    m_dockDirection= DockDown;
}

void
ProjectWindow::onGUI(const UT::Timestep &)
{
    UT::Window *window = imguiContext()->window();
    if (window == nullptr)
    {
        UT_LOG_ERROR("IMGUI context is not attached to a window.");
        return;
    }

    GR::Renderer2D::Statistics stats = GR::Renderer2D::statistics();

    ImGui::Text("Render2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.m_drawCalls);
    ImGui::Text("Quads: %d", stats.m_quadCount);
    ImGui::Text("Vertices: %d", stats.totalVertexCount());
    ImGui::Text("Indices: %d", stats.totalIndexCount());
}

} // namespace dogb