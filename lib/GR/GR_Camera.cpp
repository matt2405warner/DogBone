//
// Created by matt on 2020-07-28.
//

#include "GR_Camera.h"

#include <UT/UT_Assert.h>

#include <glm/gtc/matrix_transform.hpp>

namespace dogb::GR
{
Camera::Camera() :
    m_position (0.0f),
    m_rotation (0.0f),
    m_orthographic(true),
    m_rect(0.0f),
    m_activeTexture(nullptr)
{}
void
Camera::calculateMatricies(glm::mat4 &proj, glm::mat4 &view) const
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
                          glm::rotate(
                                  glm::mat4(1.0f), glm::radians(m_rotation),
                                  glm::vec3(0, 0, 1));

    view = glm::inverse(transform);

    if (m_orthographic)
    {
        proj = glm::ortho(m_rect.x, m_rect.y, m_rect.z, m_rect.w, -1.0f, 1.0f);
    }
    else
    {
        UT_ASSERT_MSG(false, "Unsupported camera type..");
    }
}
}