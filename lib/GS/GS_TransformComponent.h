//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_TRANSFORMCOMPONENT_H
#define DOGBONE_GS_TRANSFORMCOMPONENT_H

#include "GS_API.h"

#include <glm/glm.hpp>

namespace dogb::GS
{

struct DB_GS_API TransformComponent
{
    TransformComponent() = default;
    TransformComponent(const glm::mat4& transform) :
        m_transform(transform)
    {}

    operator glm::mat4 & () { return m_transform; }
    operator const glm::mat4 & () const { return m_transform; }

    glm::mat4 m_transform{1.0f};
};

}

#endif // DOGBONE_GS_TRANSFORMCOMPONENT_H
