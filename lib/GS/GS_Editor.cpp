//
// Created by matt on 2020-07-26.
//

#include "GS_Editor.h"

#include <UT/UT_Application.h>
#include <UT/UT_Window.h>

namespace dogb::GS
{
Editor &
Editor::instance()
{
    static Editor _instance;
    return _instance;
}
void
Editor::initialize()
{
    UT::Application* app =  UT::AppInterface::mainApp();
    UT::Window* window = app->mainWindow();

    GR::Framebuffer::Specification spec;
    spec.m_width = window->width();
    spec.m_height = window->height();

    // Generate our frame buffer
    m_frameBuffer = GR::Framebuffer::create(spec);
}
void
Editor::shutdown()
{
    m_frameBuffer.reset();
}

}
