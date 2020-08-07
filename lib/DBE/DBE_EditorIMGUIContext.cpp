//
// Created by matt on 2020-07-28.
//

#include "DBE_EditorIMGUIContext.h"

#include "DBE_Editor.h"

#include <GS/GS_World.h>

namespace dogb::DBE
{
void
EditorIMGUIContext::onGUI()
{
    Editor::drawMenus();
}
}
