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
    ImGui::Checkbox("Orthographic", &m_orthographic);
    // This isnt the correct way to display this property
    ImGui::DragFloat4("Rect", glm::value_ptr(m_rect));
    // TODO: allow to specify the texture
}
}