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

DB_IMGUI_API void EditorDrawTransform(glm::mat4& value);

}


#endif // DOGBONE_IMGUI_EDITOR_H
