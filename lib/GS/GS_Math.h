//
// Created by matt on 2020-12-13.
//

#ifndef DOGBONE_GS_MATH_H
#define DOGBONE_GS_MATH_H

#include "GS_API.h"

#include <glm/glm.hpp>

namespace dogb::GS
{
DB_GS_API bool decomposeTransform(
        const glm::mat4 &transform,
        glm::vec3 &translation,
        glm::vec3 &rotation,
        glm::vec3 &scale);
}

#endif // DOGBONE_GS_MATH_H
