//
// Created by matt on 2020-07-29.
//

#ifndef DOGBONE_GS_CAMERA_H
#define DOGBONE_GS_CAMERA_H

#include "GS_API.h"

#include <GR/GR_Camera.h>

#include <CE/CE_Asset.h>

#include <rttr/type.h>

namespace dogb::GS
{
class DB_GS_API Camera : public GR::Camera, public CE::Asset
{
    RTTR_ENABLE(GR::Camera, CE::Asset)
public:
    Camera();

    void onGUI();

    void setViewportSize(uint32_t width, uint32_t height);

    void setOrthographic(float size, float near, float far);
    void setPerspective(float fov, float near, float far);

    void setOrthographicSize(float size)
    {
        UT_ASSERT(isOrthographic());
        m_orthographicSize = size;
        recalculateProjection();
    }
    void setPerspectiveFOV(float fov)
    {
        UT_ASSERT(!isOrthographic());
        m_fov = fov;
        recalculateProjection();
    }
    void setNear(float near)
    {
        if (m_orthographic)
            m_orthoNear = near;
        else
            m_near = near;
        recalculateProjection();
    }
    void setFar(float far)
    {
        if (m_orthographic)
            m_orthoFar = far;
        else
            m_far = far;
        recalculateProjection();
    }
    [[nodiscard]] float far() const
    {
        return m_orthographic ? m_orthoFar : m_far;
    }
    [[nodiscard]] float near() const
    {
        return m_orthographic ? m_orthoNear : m_near;
    }
    [[nodiscard]] bool isOrthographic() const
    {
        return m_orthographic;
    }
    void setOrthographic(bool ortho)
    {
        m_orthographic = ortho;
        recalculateProjection();
    }
    [[nodiscard]] float fov() const
    {
        UT_ASSERT(!isOrthographic());
        return m_fov;
    }
    [[nodiscard]] float orthoSize() const
    {
        UT_ASSERT(isOrthographic());
        return m_orthographicSize;
    }

    std::shared_ptr<GR::Framebuffer> m_activeTexture;

    bool m_isDisabled = false;
    bool m_fixedAspectRatio = false;
    glm::vec4 m_background = {0.3f, 0.3f, 0.9f, 1.0f};

protected:
    bool m_orthographic = true;

    // Perspective values
    float m_near;
    float m_far;
    float m_fov;
    // Ortho values
    float m_orthoNear;
    float m_orthoFar;
    float m_orthographicSize;

    float m_aspect;

private:
    void recalculateProjection();
};
}

#endif // DOGBONE_GS_CAMERA_H
