//
// Created by matt on 2020-07-29.
//

#include "GS_Camera.h"

#include <UT/UT_Assert.h>

#include <rttr/registration>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace dogb::GS
{
Camera::Camera() :
        m_activeTexture(nullptr),
        m_orthographic(true),
        m_near(0.01f),
        m_far(1000.0f),
        m_fov(glm::radians(45.0f)),
        m_orthoNear(-1.0f),
        m_orthoFar(1.0f),
        m_orthographicSize(10.0f),
        m_aspect(1.78f)
{
    recalculateProjection();
}

void
Camera::onGUI()
{
    ImGui::Checkbox("Disabled", &m_isDisabled);
    ImGui::ColorEdit4("Background", glm::value_ptr(m_background));

    const char *perspective = "Perspective";
    const char *ortho = "Orthographic";
    if (ImGui::BeginCombo("Projection", m_orthographic ? ortho : perspective))
    {
        if (ImGui::Selectable(perspective))
        {
            m_orthographic = false;
            recalculateProjection();
        }
        if (ImGui::Selectable(ortho))
        {
            m_orthographic = true;
            recalculateProjection();
        }

        ImGui::EndCombo();
    }

    ImGui::Text("Clipping Planes");

    if (m_orthographic)
    {
        float size = orthoSize();
        if (ImGui::DragFloat("Orthographic Size", &size))
            setOrthographicSize(size);
    }
    else
    {
        float _fov = glm::degrees(fov());
        if (ImGui::DragFloat("FOV", &_fov))
            setPerspectiveFOV(glm::radians(_fov));
    }
    float _near = near();
    if (ImGui::DragFloat("Near", &_near))
        setNear(_near);
    float _far = far();
    if (ImGui::DragFloat("Far", &_far))
        setFar(_far);

    ImGui::Checkbox("Fixed Aspect", &m_fixedAspectRatio);
    // This isnt the correct way to display this property
    //ImGui::DragFloat4("Viewport Rect", glm::value_ptr(m_rect));

    // TODO: allow to specify the texture
}

void
Camera::setViewportSize(uint32_t width, uint32_t height)
{
    m_aspect = (float)width / (float)height;

    recalculateProjection();
}
void
Camera::setOrthographic(float size, float near, float far)
{
    m_orthographic = true;
    m_orthographicSize = size;
    m_orthoNear = near;
    m_orthoFar = far;

    recalculateProjection();
}
void
Camera::setPerspective(float fov, float near, float far)
{
    m_orthographic = false;
    m_fov = fov;
    m_near = near;
    m_far = far;

    recalculateProjection();
}
void
Camera::recalculateProjection()
{
    if (m_orthographic)
    {
        float left= -m_orthographicSize * m_aspect * 0.5f;
        float right = m_orthographicSize * m_aspect * 0.5f;
        float bottom = -m_orthographicSize * 0.5f;
        float top = m_orthographicSize * 0.5f;

        m_projection = glm::ortho(left, right, bottom, top, m_orthoNear, m_orthoFar);
    }
    else
    {
        m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
    }
}
} // namespace dogb::GS