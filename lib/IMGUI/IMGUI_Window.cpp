//
// Created by matt on 2020-07-11.
//

#include "IMGUI_Window.h"

#include "IMGUI_SubSystem.h"

#include <UT/UT_Engine.h>

#include <rttr/registration>

namespace dogb::IMGUI
{
SubSystemContext *
Window::imguiContext() const
{
    return m_context;
}

} // namespace dogb::IMGUI