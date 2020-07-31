//
// Created by matt on 2020-07-23.
//

#include "GS_TransformComponent.h"

#include <IMGUI/IMGUI_Editor.h>

void
dogb::GS::TransformComponent::onGUI()
{
    IMGUI::EditorDrawTransform(m_transform);
}