//
// Created by matt on 2020-07-23.
//

#include "GS_Mesh2DComponent.h"

#include <GR/GR_Renderer2D.h>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace dogb::GS
{
AUTOREGISTER_TYPE(Mesh2DComponent)

void
Mesh2DComponent::onGUI()
{
    ImGui::ColorEdit4("color", glm::value_ptr(m_color.toVec4()));
}
void
Mesh2DComponent::serialize(GS_YAML::Emitter& emitter) const
{
    emitter << GS_YAML::Key << "m_color" << GS_YAML::Value << m_color;
}
void
Mesh2DComponent::deserialize(GS_YAML::Node& node)
{
    m_color = node["m_color"].as<glm::vec4>();
}
void
Mesh2DComponent::draw(const GS::Entity& ent, const glm::mat4& transform)
{
    GR::Renderer2D::startDrawQuad(transform);
    GR::Renderer2D::pushEntity(static_cast<int>(ent.handle()));
    GR::Renderer2D::pushTexture(nullptr /*text*/, m_color);
    GR::Renderer2D::endDrawQuad();
}

}
