//
// Created by matt on 2020-07-29.
//

#include "GS_CameraComponent.h"
#include "GS_Editor.h"

namespace dogb::GS
{
void
CameraComponent::onGUI()
{
    if (!m_camera)
        return;

    m_camera->onGUI();
}
}
