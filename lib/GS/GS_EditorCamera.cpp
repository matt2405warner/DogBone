//
// Created by matt on 2020-12-23.
//

#include "GS_EditorCamera.h"

#include <CE/CE_Input.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dogb::GS
{
EditorCamera::EditorCamera(
        float fov,
        float aspect_ratio,
        float near_clip,
        float far_clip)
    : GR::Camera(glm::perspective(
              glm::radians(fov),
              aspect_ratio,
              near_clip,
              far_clip))
    , m_fov(fov)
    , m_aspectRatio(aspect_ratio)
    , m_nearClip(near_clip)
    , m_farClip(far_clip)
{
}

glm::vec3
EditorCamera::up() const
{
    return glm::rotate(orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::vec3
EditorCamera::right() const
{
    return glm::rotate(orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}
glm::vec3
EditorCamera::forward() const
{
    return glm::rotate(orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}
glm::quat
EditorCamera::orientation() const
{
    return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
}
void
EditorCamera::updateProjection_()
{
    m_aspectRatio = m_viewportWidth / m_viewportHeight;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
}
void
EditorCamera::updateView_()
{
    m_position = calculatePosition_();

    glm::quat orient = orientation();
    m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orient);
    m_viewMatrix = glm::inverse(m_viewMatrix);
}

void
EditorCamera::mousePan_(const glm::vec2 &delta)
{
    auto [xspeed, yspeed] = panSpeed_();
    m_focalPoint += -right() * delta.x * xspeed * m_distance;
    m_focalPoint += up() * delta.y * yspeed * m_distance;
}

void
EditorCamera::mouseRotate_(const glm::vec2 &delta)
{
    float yaw_sign = up().y < 0 ? -1.0f : 1.0f;
    m_yaw += yaw_sign * delta.x * rotationSpeed_();
    m_pitch += delta.y * rotationSpeed_();
}

void
EditorCamera::mouseZoom_(float delta)
{
    m_distance -= delta * zoomSpeed_();
    if (m_distance < 1.0f)
    {
        m_focalPoint += forward();
        m_distance = 1.0f;
    }
}

glm::vec3
EditorCamera::calculatePosition_() const
{
    return m_focalPoint - forward() * m_distance;
}

std::pair<float, float>
EditorCamera::panSpeed_() const
{
    float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
    float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_viewportHeight / 1000.0f, 2.4f);
    float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {x_factor, y_factor};
}

float
EditorCamera::rotationSpeed_() const
{
    return 0.8f;
}

float
EditorCamera::zoomSpeed_() const
{
    float distance = m_distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f);
    return speed;
}

void
EditorCamera::onUpdate(const UT::Timestep &)
{
    if (CE::Input::IsKeyPressed(CE::Input::KEY_LEFT_CTRL))
    {
        const glm::vec2 & mouse{CE::Input::GetMouseX(), CE::Input::GetMouseY()};
        glm::vec2  delta = (mouse - m_initialMousePosition) * 0.003f;
        m_initialMousePosition = mouse;

        if (CE::Input::IsButtonPressed(CE::Input::MOUSE_BUTTON_MIDDLE))
        {
            mousePan_(delta);
        }
        else if (CE::Input::IsButtonPressed(CE::Input::MOUSE_BUTTON_LEFT))
        {
            mouseRotate_(delta);
        }
        else if (CE::Input::IsButtonPressed(CE::Input::MOUSE_BUTTON_RIGHT))
        {
            mouseZoom_(delta.y);
        }
    }

    updateView_();
}

void
EditorCamera::onMouseScroll(float /*x*/, float y)
{
    float delta = y * 0.1f;
    mouseZoom_(delta);
    updateView_();
}

}