//
// Created by matt on 2020-07-13.
//

#include "GS_CameraController.h"

#include "GS_World.h"

#include <CE/CE_Input.h>

#include <imgui.h>

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

} // namespace dogb::GS