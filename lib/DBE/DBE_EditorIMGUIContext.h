//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_DBE_EDITORIMGUICONTEXT_H
#define DOGBONE_DBE_EDITORIMGUICONTEXT_H

#include "DBE_API.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

namespace dogb::DBE
{

class DB_DBE_API EditorIMGUIContext : public IMGUI::SubSystemContext
{
public:
    using parent = IMGUI::SubSystemContext;
    using parent::parent;
    void onGUI() override ;
};

}

#endif // DOGBONE_DBE_EDITORIMGUICONTEXT_H
