//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GS_COLOR_H
#define DOGBONE_GS_COLOR_H

#include "GR_API.h"

#include <glm/glm.hpp>

#include <rttr/type>

namespace dogb::GR
{
class DB_GR_API Color3 : public glm::vec3
{
public:
    using parent = glm::vec3;

    using parent::parent;
    Color3() = default;
    explicit Color3(const parent& v) : parent(v) {}

    virtual ~Color3() = default;

    [[nodiscard]] const glm::vec3 & toVec3() const { return *this; }
    glm::vec3 & toVec3() { return *this; }
};

class DB_GR_API Color : public glm::vec4
{
public:
    using parent = glm::vec4;

    using parent::parent;
    Color() = default;

    Color(const Color& color) = default;
    explicit Color(const parent& v) : parent(v) {}

    virtual ~Color() = default;

    Color& operator=(const Color& color) = default;
    Color& operator=(const parent& v);

    [[nodiscard]] const glm::vec4 & toVec4() const { return *this; }
    glm::vec4 & toVec4() { return *this; }
};
}

#endif // DOGBONE_GS_COLOR_H
