//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_TRANSFORMCOMPONENT_H
#define DOGBONE_GS_TRANSFORMCOMPONENT_H

#include "GS_API.h"

#include "GS_Entity.h"

#include <glm/glm.hpp>

#include <rttr/type>

#include <vector>

namespace dogb::GS
{

struct DB_GS_API TransformComponent
{
public:
    static constexpr const char* theGUIName = "Transform";

    TransformComponent() = default;
    TransformComponent(const glm::mat4& transform) :
        m_transform(transform)
    {}
    virtual ~TransformComponent() = default;

    operator glm::mat4 & () { return m_transform; }
    operator const glm::mat4 & () const { return m_transform; }

    void onGUI();

    glm::mat4 m_transform{1.0f};

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
