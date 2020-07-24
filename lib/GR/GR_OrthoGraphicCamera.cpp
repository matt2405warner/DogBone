//
// Created by matt on 2020-07-11.
//

#include "GR_OrthoGraphicCamera.h"

#include "GR_OrthoGraphicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace dogb::GR
{
OrthoGraphicCamera::OrthoGraphicCamera(
        float left,
        float right,
        float bottom,
        float top)
    : m_viewMatrix(1.0f), m_position({0.0f, 0.0f, 0.0f})
{
    setProjection(left, right, bottom, top);
}
void
OrthoGraphicCamera::recalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
                          glm::rotate(
                                  glm::mat4(1.0f), glm::radians(m_rotation),
                                  glm::vec3(0, 0, 1));

    m_viewMatrix = glm::inverse(transform);
    m_viewProjMatrix = m_projMatrix * m_viewMatrix;
}
void
OrthoGraphicCamera::setProjection(
        float left,
        float right,
        float bottom,
        float top)
{
    m_projMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_viewProjMatrix = m_projMatrix * m_viewMatrix;
}

} // namespace dogb::GR