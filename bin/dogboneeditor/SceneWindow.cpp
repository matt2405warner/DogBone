//
// Created by matt on 2020-07-11.
//

#include "SceneWindow.h"

#include "TestContext_2D.h"

#include <imgui.h>

namespace dogb
{
void
SceneWindow::onStart()
{
    m_title = "Scene";

    m_style.m_padding = ImVec2{0, 0};
}

void
SceneWindow::onGUI()
{
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();

    if (m_viewportSize.x != viewport_size.x ||
        m_viewportSize.y != viewport_size.y)
    {
        m_viewportSize = {viewport_size.x, viewport_size.y};
        if (m_viewportSize.x <= 0)
            m_viewportSize.x = 1;
        if (m_viewportSize.y <= 0)
            m_viewportSize.y = 1;

        m_ctx->m_cameraController.resize(m_viewportSize.x, m_viewportSize.y);
        m_framebuffer->resize(
                static_cast<uint32_t>(m_viewportSize.x),
                static_cast<uint32_t>(m_viewportSize.y));
    }

    uint32_t tex_id = m_framebuffer->colorAttachmentRendererID();

    ImGui::Image(
            reinterpret_cast<void *>(tex_id),
            ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});
}

} // namespace dogb