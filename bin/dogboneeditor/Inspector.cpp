//
// Created by matt on 2020-07-11.
//

#include "Inspector.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace dogb
{
void
Inspector::onStart()
{
    m_title = "Inspector";
}

void
Inspector::onGUI()
{
    UT::Window *window = imguiContext()->window();
    if (window == nullptr)
    {
        UT_LOG_ERROR("IMGUI context is not attached to a window.");
        return;
    }

    //int win_width = window->width();
    //int win_height = window->height();

    ImGui::Text("Hello World.");
}

} // namespace dogb