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
    UT::Application *app = UT::AppInterface::mainApp();
    UT::Window *window = app->mainWindow();

    GR::Framebuffer::Specification spec;
    spec.m_attachments = {
            GR::Framebuffer::TextureFormat::RGBA8,
            GR::Framebuffer::TextureFormat::RED_INTEGER,
            GR::Framebuffer::TextureFormat::Depth};
    spec.m_width = window->width();
    spec.m_height = window->height();

    // Generate our frame buffer
    m_frameBuffer = GR::Framebuffer::create(spec);

    m_camera = EditorCamera(
            30.0f, (float)window->width() / (float)window->height(), 0.1f,
            1000.0f);
}
void
Editor::shutdown()
{
    m_frameBuffer.reset();
}
void
Editor::onUpdate(const UT::Timestep &ts)
{
    m_camera.onUpdate(ts);
}

} // namespace dogb::GS
