//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_TRANSFORMCOMPONENT_H
#define DOGBONE_GS_TRANSFORMCOMPONENT_H

#include "GS_API.h"

#include "GS_Entity.h"
#include "GS_YAML.h"
#include "GS_ComponentTypeRegistry.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <rttr/type>

#include <vector>

namespace dogb::GS
{

struct DB_GS_API TransformComponent
{
public:
    REFLECT_COMPONENT(TransformComponent, "Transform")

    TransformComponent();
    virtual ~TransformComponent() = default;

    void onGUI();
    void serialize(GS_YAML::Emitter& emitter) const;
    void deserialize(GS_YAML::Node& node);

    [[nodiscard]] glm::mat4 transform() const;

    void removeChild(const Entity& e);

    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    Entity m_root;
    Entity m_parent;
    std::vector<Entity> m_children;
};

template <>
constexpr GUI_TRAITS GUITypeTraits<TransformComponent>()
{
    return GUI_NO_REMOVE;
}

}

#endif // DOGBONE_GS_TRANSFORMCOMPONENT_H
