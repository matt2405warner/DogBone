//
// Created by matt on 2020-07-23.
//

#include "GS_TransformComponent.h"

#include <IMGUI/IMGUI_Editor.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

AUTOREGISTER_TYPE(TransformComponent)

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
#if 1
    glm::mat4 rotation = glm::toMat4(glm::quat(m_rotation));
#else
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, {1, 0, 0})
                         * glm::rotate(glm::mat4(1.0f), m_rotation.y, {0, 1, 0})
                         * glm::rotate(glm::mat4(1.0f), m_rotation.z, {0, 0, 1});
#endif

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
void
TransformComponent::serialize(YAML::Emitter &emitter) const
{
    // TODO: we need to capture the transform hierarchy
    emitter << YAML::Key << "m_translation" << YAML::Value << m_translation;
    emitter << YAML::Key << "m_rotation" << YAML::Value << m_rotation;
    emitter << YAML::Key << "m_scale" << YAML::Value << m_scale;
}
void
TransformComponent::deserialize(GS_YAML::Node &node)
{
    m_translation = node["m_translation"].as<glm::vec3>();
    m_rotation = node["m_rotation"].as<glm::vec3 >();
    m_scale = node["m_scale"].as<glm::vec3>();
}

} // namespace dogb::GS