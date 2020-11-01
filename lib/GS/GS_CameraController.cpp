//
// Created by matt on 2020-07-13.
//

#include "GS_CameraController.h"

#include "GS_World.h"

#include <CE/CE_Input.h>

#include <imgui.h>

AUTOREGISTER_TYPE(CameraController)

namespace dogb::GS
{

CameraController::CameraController()
    : m_moveSpeed(1.0f)
    , m_rotateSpeed(20.0f)
    , m_enableRotate(true)
{
}

void CameraController::onGUI()
{
    ImGui::DragFloat("Move Speed", &m_moveSpeed);
    ImGui::DragFloat("Rotate Speed", &m_rotateSpeed);
    ImGui::Checkbox("Enable rotation", &m_enableRotate);
}
void
CameraController::serialize(GS_YAML::Emitter &emitter) const
{
    emitter << GS_YAML::Key << "m_moveSpeed" << GS_YAML::Value << m_moveSpeed;
    emitter << GS_YAML::Key << "m_rotateSpeed" << GS_YAML::Value << m_rotateSpeed;
    emitter << GS_YAML::Key << "m_enableRotate" << GS_YAML::Value << m_enableRotate;
}
void
CameraController::deserialize(GS_YAML::Node &node)
{
    m_moveSpeed = node["m_moveSpeed"].as<float>();
    m_rotateSpeed = node["m_rotateSpeed"].as<float>();
    m_enableRotate = node["m_enableRotate"].as<bool>();
}

} // namespace dogb::GS