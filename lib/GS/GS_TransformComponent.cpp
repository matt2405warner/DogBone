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
    euler_rotation = glm::degrees(euler_rotation);

    ImGui::DragFloat3("Position", glm::value_ptr(m_position));
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler_rotation), 1.0f))
    {
        if (euler_rotation.y >= 89.0f)
            euler_rotation.y -= 89.0f * 2.0f;
        else if (euler_rotation.y <= -89.0f)
            euler_rotation.y += 89.0f * 2.0f;

        if (euler_rotation.x >= 180.0f)
            euler_rotation.x -= 360.0f;
        else if (euler_rotation.x <= 180.0f)
            euler_rotation.x += 360.0f;

        if (euler_rotation.z >= 180.0f)
            euler_rotation.z -= 360.0f;
        else if (euler_rotation.z <= 180.0f)
            euler_rotation.z += 360.0f;

        glm::quat q_x_axis = glm::angleAxis(
                glm::radians(euler_rotation.x), glm::vec3{1.0f, 0.0f, 0.0f});
        glm::quat q_y_axis = glm::angleAxis(
                glm::radians(euler_rotation.y), glm::vec3{0.0, 1.0f, 0.0f});
        glm::quat q_z_axis = glm::angleAxis(
                glm::radians(euler_rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});

        m_rotation = q_z_axis * q_y_axis * q_x_axis;
    }
    ImGui::DragFloat3("Scale", glm::value_ptr(m_scale));
}
glm::mat4
TransformComponent::transform() const
{
    return glm::translate(glm::mat4(), m_position) * glm::toMat4(m_rotation) *
           glm::scale(glm::mat4(), m_scale);
}
void
TransformComponent::removeChild(const Entity &e)
{
    for (auto it = m_children.begin(); it != m_children.end(); it++)
    {
        if ((*it) == e)
        {
            it = m_children.erase(it);
            break;
        }
    }
}

} // namespace dogb::GS