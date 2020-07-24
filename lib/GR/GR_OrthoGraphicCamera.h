//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_ORTHOGRAPHICCAMERA_H
#define DOGBONE_GR_ORTHOGRAPHICCAMERA_H

#include "GR_API.h"

#include <glm/glm.hpp>

namespace dogb::GR
{
// This is currently just a 2D camera
class GR_API OrthoGraphicCamera
{
public:
    OrthoGraphicCamera(float left, float right, float bottom, float top);

    void setPosition(const glm::vec3 &position)
    {
        m_position = position;
        recalculateViewMatrix();
    }
    void setRotation(float rotation)
    {
        m_rotation = rotation;
        recalculateViewMatrix();
    }

    void setProjection(float left, float right, float bottom, float top);

    [[nodiscard]] const glm::vec3 &position() const { return m_position; }
    [[nodiscard]] float rotation() const { return m_rotation; }

    [[nodiscard]] const glm::mat4 &projectionMatrix() const { return m_projMatrix; }
    [[nodiscard]] const glm::mat4 &viewMatrix() const { return m_viewMatrix; }
    [[nodiscard]] const glm::mat4 &viewProjectionMatrix() const { return m_viewProjMatrix; }

private:
    void recalculateViewMatrix();

    glm::mat4 m_projMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_viewProjMatrix;

    glm::vec3 m_position;
    float m_rotation = 0.0f;
};
} // namespace rne::UT

#endif // DOGBONE_GR_ORTHOGRAPHICCAMERA_H
