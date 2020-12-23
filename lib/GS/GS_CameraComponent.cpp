//
// Created by matt on 2020-07-29.
//

#include "GS_CameraComponent.h"

#include "GS_Editor.h"
#include "GS_World.h"

#include <UT/UT_Application.h>

AUTOREGISTER_TYPE(CameraComponent)

namespace dogb::GS
{
CameraComponent::CameraComponent()
{
    m_camera = std::make_shared<GS::Camera>();
    m_camera->m_primary = true;
    m_camera->setOrthographic(false);

    UT::Application* app = UT::AppInterface::mainApp();
    UT::Window* window = app->mainWindow();

    U32 width = window->width();
    U32 height = window->height();

    m_camera->setViewportSize(width, height);
}

void
CameraComponent::onGUI()
{
    if (!m_camera)
        return;

    m_camera->onGUI();
}
void
CameraComponent::serialize(YAML::Emitter &emitter) const
{
    emitter << YAML::Key << "m_orthographic" << YAML::Value << m_camera->isOrthographic();
    // Perspective values
    emitter << YAML::Key << "m_fov" << YAML::Value << m_camera->fov();
    emitter << YAML::Key << "m_near" << YAML::Value << m_camera->perspectiveNear();
    emitter << YAML::Key << "m_far" << YAML::Value << m_camera->perspectiveFar();
    // Orthographic values
    emitter << YAML::Key << "m_orthographicSize" << YAML::Value << m_camera->orthoSize();
    emitter << YAML::Key << "m_orthoNear" << YAML::Value << m_camera->orthoNear();
    emitter << YAML::Key << "m_orthoFar" << YAML::Value << m_camera->orthoFar();
    // Extra values
    emitter << YAML::Key << "m_disabled" << YAML::Value << m_camera->m_isDisabled;
    emitter << YAML::Key << "m_fixedAspect" << YAML::Value << m_camera->m_fixedAspectRatio;
    emitter << YAML::Key << "m_background" << YAML::Value << m_camera->m_background;
    emitter << YAML::Key << "m_primary" << YAML::Value << m_camera->m_primary;
}
void
CameraComponent::deserialize(GS_YAML::Node &node)
{
    // Perspective values
    float fov = node["m_fov"].as<float>();
    float near = node["m_near"].as<float>();
    float far = node["m_far"].as<float>();
    m_camera->setPerspective(fov, near, far);
    // Orthographic values
    float ortho_size = node["m_orthographicSize"].as<float>();
    float ortho_near = node["m_orthoNear"].as<float>();
    float ortho_far = node["m_orthoFar"].as<float>();
    m_camera->setOrthographic(ortho_size, ortho_near, ortho_far);
    // Extra values
    m_camera->m_isDisabled = node["m_disabled"].as<bool>();
    m_camera->m_fixedAspectRatio = node["m_fixedAspect"].as<bool>();
    m_camera->m_background = node["m_background"].as<glm::vec4>();
    m_camera->setOrthographic(node["m_orthographic"].as<bool>());
    m_camera->m_primary = node["m_primary"].as<bool>();
}
}
