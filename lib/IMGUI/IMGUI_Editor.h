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

class DB_IMGUI_API Editor : public GS::EditorBase
{
public:
    static void initialize();

private:
    // Standard Types
    static void drawInt32Clb(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop);
    // GLM Types
    static void drawGLMVec3(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop);
    static void drawGLMVec4(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop);
};

}


#endif // DOGBONE_IMGUI_EDITOR_H
