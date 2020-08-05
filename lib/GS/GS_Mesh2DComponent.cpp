//
// Created by matt on 2020-07-23.
//

#include "GS_Mesh2DComponent.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace dogb::GS
{
void
dogb::GS::Mesh2DComponent::onGUI()
{
    ImGui::ColorEdit4("color", glm::value_ptr(m_color.toVec4()));
}
}
