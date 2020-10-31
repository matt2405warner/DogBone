//
// Created by matt on 2020-07-23.
//

#include "GS_TransformComponent.h"

#include <IMGUI/IMGUI_Editor.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dogb::GS
{
TransformComponent::TransformComponent()
    : m_translation({0.0f, 0.0f, 0.0f}), m_rotation({0.0f, 0.0f, 0.0f}), m_scale({1.0f, 1.0f, 1.0f})
{
}

void
TransformComponent::onGUI()
{
    IMGUI::EditorDrawTransform(m_translation, m_rotation, m_scale);
}
glm::mat4
TransformComponent::transform() const
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, {1, 0, 0})
                         * glm::rotate(glm::mat4(1.0f), m_rotation.y, {0, 1, 0})
                         * glm::rotate(glm::mat4(1.0f), m_rotation.z, {0, 0, 1});

    return glm::translate(glm::mat4(1.0f), m_translation) * rotation *
           glm::scale(glm::mat4(1.0f), m_scale);
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