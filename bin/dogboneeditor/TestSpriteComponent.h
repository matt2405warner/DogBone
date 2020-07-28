//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_TESTSPRITECOMPONENT_H
#define DOGBONE_TESTSPRITECOMPONENT_H

#include <rttr/type>

#include <glm/glm.hpp>

namespace dogb
{
class TestSpriteComponent
{
    RTTR_ENABLE()

public:
    TestSpriteComponent() = default;
    TestSpriteComponent(const glm::vec4 &color) : m_color(color) {}
    virtual ~TestSpriteComponent() = default;

    operator glm::vec4 &() { return m_color; }
    operator const glm::vec4 &() const { return m_color; }

    glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
};
}

#endif // DOGBONE_TESTSPRITECOMPONENT_H
