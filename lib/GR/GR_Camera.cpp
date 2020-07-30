//
// Created by matt on 2020-07-28.
//

#include "GR_Camera.h"

#include <UT/UT_Assert.h>

#include <glm/gtc/matrix_transform.hpp>

namespace dogb::GR
{
Camera::Camera() :
    m_orthographic(true),
    m_rect(0.0f),
    m_activeTexture(nullptr)
{}
void
Camera::calculateMatricies(glm::mat4 &proj) const
{
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