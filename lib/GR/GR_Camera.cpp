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
    m_near(-1.0f),
    m_far(1.0f),
    m_orthographicSize(10.0f),
    m_aspect(1.78f),
    m_activeTexture(nullptr)
{}
void
Camera::calculateMatricies(glm::mat4 &proj) const
{
    if (m_orthographic)
    {
        float left= -m_orthographicSize * m_aspect * 0.5f;
        float right = m_orthographicSize * m_aspect * 0.5f;
        float bottom = -m_orthographicSize * 0.5f;
        float top = m_orthographicSize * 0.5f;

        proj = glm::ortho(left, right, bottom, top, m_near, m_far);
    }
    else
    {
        UT_ASSERT_MSG(false, "Unsupported camera type..");
    }
}

void
Camera::setViewportSize(uint32_t width, uint32_t height)
{
    m_aspect = (float)width / (float)height;
}

}