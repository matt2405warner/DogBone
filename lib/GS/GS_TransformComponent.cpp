//
// Created by matt on 2020-07-23.
//

#include "GS_TransformComponent.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dogb::GS
{
TransformComponent::TransformComponent()
    : m_position({0.0f, 0.0f, 0.0f}), m_rotation(), m_scale({1.0f, 1.0f, 1.0f})
{
}

void
TransformComponent::onGUI()
{
    glm::vec3 euler_rotation = glm::eulerAngles(m_rotation);

    ImGui::DragFloat3("Position", glm::value_ptr(m_position));
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler_rotation)))
        m_rotation = glm::toQuat(glm::orientate3(euler_rotation));
    ImGui::DragFloat3("Scale", glm::value_ptr(m_scale));
}
glm::mat4
TransformComponent::transform() const
{
    return glm::translate(glm::mat4(), m_position) * glm::toMat4(m_rotation) *
           glm::scale(glm::mat4(), m_scale);
}

} // namespace dogb::GS