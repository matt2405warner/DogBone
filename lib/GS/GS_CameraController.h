//
// Created by matt on 2020-07-13.
//

#ifndef DOGBONE_GS_CAMERACONTROLLER_H
#define DOGBONE_GS_CAMERACONTROLLER_H

#include "GS_API.h"

#include <GR/GR_DesktopWindow.h>
#include <GR/GR_OrthoGraphicCamera.h>

#include <UT/UT_Timestep.h>
#include <UT/UT_EventDispatcher.h>

#include <boost/signals2.hpp>

namespace dogb::GS
{
struct GS_API CameraBounds
{
public:
    [[nodiscard]] float width() const { return m_right - m_left; }
    [[nodiscard]] float height() const { return m_top - m_bottom; }

    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
};

class GS_API CameraController
{
public:
    explicit CameraController(float aspect_ratio, bool enable_rotate = true);
    ~CameraController();

    void onUpdate(UT::Timestep ts);

    void attach(GR::DesktopWindow& window);

    void setAspectRatio(float aspect_ratio);
    void setZoomLevel(float level);

    void resize(float width, float height);

    [[nodiscard]] GR::OrthoGraphicCamera& camera() { return m_camera; }

    void setDisable(bool disable) { m_disable = disable; }
private:
    void onMouseScrolled(float x, float y);
    void onWindowResized(I32 width, I32 height);

    void refreshView();

    bool m_disable = false;

    float m_moveSpeed;
    float m_rotateSpeed;
    bool m_enableRotate;

    float m_aspectRatio{};
    float m_zoomLevel = 1.0;
    glm::vec3 m_position;
    float m_rotation;

    CameraBounds m_bounds;
    GR::OrthoGraphicCamera m_camera;

private:
    boost::signals2::connection m_resizeConnection;
    boost::signals2::connection m_scrolledConnection;
};

} // namespace dogb::GS

#endif // DOGBONE_GS_CAMERACONTROLLER_H
