//
// Created by matt on 2020-07-29.
//

#include "GS_Camera.h"

#include <rttr/registration>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace dogb::GS
{
void
Camera::onGUI()
{
    ImGui::Checkbox("Disabled", &m_isDisabled);
    ImGui::ColorEdit4("Background", glm::value_ptr(m_background));

    const char *perspective = "Perspective";
    const char *ortho = "Orthographic";
    if (ImGui::BeginCombo("Projection", m_orthographic ? ortho : perspective))
    {
        if (ImGui::Selectable(perspective))
            m_orthographic = false;
        if (ImGui::Selectable(ortho))
            m_orthographic = true;

        ImGui::EndCombo();
    }

    ImGui::Text("Clipping Planes");

    ImGui::Indent();
    ImGui::DragFloat("Near", &m_near);
    ImGui::DragFloat("Far", &m_far);
    ImGui::Unindent();

    if (m_orthographic)
    {
        ImGui::DragFloat("Orthographic Size", &m_orthographicSize);
    }

    ImGui::Checkbox("Fixed Aspect", &m_fixedAspectRatio);
    // This isnt the correct way to display this property
    //ImGui::DragFloat4("Viewport Rect", glm::value_ptr(m_rect));

    // TODO: allow to specify the texture
}
} // namespace dogb::GS