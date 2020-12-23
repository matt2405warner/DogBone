//
// Created by matt on 2020-12-23.
//

#ifndef DOGBONE_DBE_EDITORCAMERA_H
#define DOGBONE_DBE_EDITORCAMERA_H

#include "GS_API.h"

#include <GR/GR_Camera.h>

#include <UT/UT_Timestep.h>

#include <glm/glm.hpp>

namespace dogb::GS
{
class DB_GS_API EditorCamera : public GR::Camera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip);

    [[nodiscard]] inline float distance() const{ return m_distance; }
    void setDistance(float distance) { m_distance = distance; }

    void setViewportSize(float width, float height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
        updateProjection_();
    }

    void onUpdate(const UT::Timestep &ts);
    void onMouseScroll(float x, float y);

    [[nodiscard]] const glm::mat4& viewMatrix() const { return m_viewMatrix; }
    [[nodiscard]] glm::mat4 viewProjection() const { return m_projection * m_viewMatrix; }

    [[nodiscard]] glm::vec3 up() const;
    [[nodiscard]] glm::vec3 right() const;
    [[nodiscard]] glm::vec3 forward() const;
    [[nodiscard]] const glm::vec3& position() const { return m_position; }
    [[nodiscard]] glm::quat orientation() const;

    [[nodiscard]] float pitch() const { return m_pitch; }
    [[nodiscard]] float yaw() const { return m_yaw; }

private:
    void updateProjection_();
    void updateView_();

    void mousePan_(const glm::vec2& delta);
    void mouseRotate_(const glm::vec2& delta);
    void mouseZoom_(float delta);

    [[nodiscard]] glm::vec3 calculatePosition_() const;
    [[nodiscard]] std::pair<float, float> panSpeed_() const;
    [[nodiscard]] float rotationSpeed_() const;
    [[nodiscard]] float zoomSpeed_() const;
private:
    float m_distance = 10.0f;
    float m_viewportWidth = 1280;
    float m_viewportHeight = 720;
    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    float m_fov = 45.0f;
    float m_aspectRatio = 1.778f;
    float m_nearClip = 0.1f;
    float m_farClip = 1000.0f;

    glm::mat4 m_viewMatrix;
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_focalPoint = {0.0f, 0.0f, 0.0f};

    glm::vec2 m_initialMousePosition;
};
} // namespace dogb::DBE

#endif // DOGBONE_DBE_EDITORCAMERA_H
