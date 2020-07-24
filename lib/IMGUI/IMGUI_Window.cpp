//
// Created by matt on 2020-07-11.
//

#include "IMGUI_Window.h"

#include "IMGUI_SubSystem.h"

#include <UT/UT_Engine.h>

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<dogb::IMGUI::Window>("dogb::IMGUI::Window");
}

namespace dogb::IMGUI
{
IMGUI::SubSystemContext *
IMGUI::Window::imguiContext() const
{
    return m_context;
}

} // namespace dogb::IMGUI