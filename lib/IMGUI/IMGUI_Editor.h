//
// Created by matt on 2020-07-26.
//

#ifndef DOGBONE_IMGUI_EDITOR_H
#define DOGBONE_IMGUI_EDITOR_H

#include "IMGUI_API.h"

#include <GS/GS_Editor.h>

#include <glm/glm.hpp>

#include <rttr/type.h>

namespace dogb::IMGUI
{

DB_IMGUI_API void EditorDrawTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
DB_IMGUI_API void EditorDrawVec3Control(const std::string& label, glm::vec3& value, float reset_value = 0.0f, float column_width = 100.0f);
}


#endif // DOGBONE_IMGUI_EDITOR_H
