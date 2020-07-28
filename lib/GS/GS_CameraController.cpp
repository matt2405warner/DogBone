//
// Created by matt on 2020-07-13.
//

#include "GS_CameraController.h"

#include "GS_World.h"

#include <CE/CE_Input.h>

namespace dogb::GS
{
CameraController::CameraController(float aspect_ratio, bool enable_rotate)
    : m_moveSpeed(1.0f)
    , m_rotateSpeed(20.0f)
    , m_enableRotate(enable_rotate)
    , m_aspectRatio(aspect_ratio)
    , m_zoomLevel(1.0f)
    , m_position(0.0f)
    , m_rotation(0.0f)
{
    World& world = World::instance();
    world.mainCamera()->m_orthographic = true;
    world.mainCamera()->m_rect = glm::vec4(-m_aspectRatio * m_zoomLevel,
                                m_aspectRatio * m_zoomLevel,
                                -m_zoomLevel,
                                m_zoomLevel);
}

CameraController::~CameraController()
{
    m_scrolledConnection.disconnect();
    m_resizeConnection.disconnect();
}

void
CameraController::onUpdate(UT::Timestep timestep)
{
    float ts = timestep.seconds();

    using namespace CE::Input;
    if (IsKeyPressed(KeyType::KEY_A))
    {
        m_position.x -= glm::cos(glm::radians(m_rotation)) * m_moveSpeed * ts;
        m_position.y -= glm::sin(glm::radians(m_rotation)) * m_moveSpeed * ts;
    }
    else if (IsKeyPressed(KeyType::KEY_D))
    {
        m_position.x += glm::cos(glm::radians(m_rotation)) * m_moveSpeed * ts;
        m_position.y += glm::sin(glm::radians(m_rotation)) * m_moveSpeed * ts;
    }

    if (IsKeyPressed(KeyType::KEY_W))
    {
        m_position.x += -glm::sin(glm::radians(m_rotation)) * m_moveSpeed * ts;
        m_position.y += glm::cos(glm::radians(m_rotation)) * m_moveSpeed * ts;
    }
    else if (IsKeyPressed(KeyType::KEY_S))
    {
        m_position.x -= -glm::sin(glm::radians(m_rotation)) * m_moveSpeed * ts;
        m_position.y -= glm::cos(glm::radians(m_rotation)) * m_moveSpeed * ts;
    }

    World& world = World::instance();

    if (m_enableRotate)
    {
        if (IsKeyPressed(KeyType::KEY_Q))
            m_rotation += m_rotateSpeed * ts;
        else if (IsKeyPressed(KeyType::KEY_E))
            m_rotation -= m_rotateSpeed * ts;

        if (m_rotation > 180.0f)
            m_rotation -= 360.0f;
        else if (m_rotation <= -180.0f)
            m_rotation += 360.0f;

        world.mainCamera()->m_rotation = m_rotation;
    }

    world.mainCamera()->m_position = m_position;
    m_moveSpeed = m_zoomLevel;
}

void
CameraController::attach(GR::DesktopWindow &window)
{
    using namespace boost::placeholders;
    m_resizeConnection = window.m_resizeSignal.connect(
            [this](auto &&PH1, auto &&PH2) { onWindowResized(PH1, PH2); });
    m_scrolledConnection = window.m_scrolledSignal.connect(
            [this](auto &&PH1, auto &&PH2) { onMouseScrolled(PH1, PH2); });
}

void
CameraController::onMouseScrolled(float, float y)
{
    if (m_disable)
        return;

    m_zoomLevel -= y;
    m_zoomLevel = std::max(m_zoomLevel, 0.25f);
    refreshView();
}

void
CameraController::onWindowResized(I32 width, I32 height)
{
    if (m_disable)
        return;

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    refreshView();
}
void
CameraController::setAspectRatio(float aspect_ratio)
{
    m_aspectRatio = aspect_ratio;
    refreshView();
}

void
CameraController::setZoomLevel(float level)
{
    m_zoomLevel = level;
    refreshView();
}

void
CameraController::refreshView()
{
    World& world = World::instance();

    m_bounds = {-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
                -m_zoomLevel, m_zoomLevel};
    world.mainCamera()->m_rect = glm::vec4(m_bounds.m_left, m_bounds.m_right, m_bounds.m_bottom, m_bounds.m_top);
}
void
CameraController::resize(float width, float height)
{
    m_aspectRatio = width / height;
    refreshView();
}

} // namespace dogb::GS