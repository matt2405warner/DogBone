//
// Created by matt on 2020-07-11.
//

#include "Inspector.h"

#include "Editor.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GS/GS_Scene.h>
#include <GS/GS_EntityManager.h>
#include <GS/GS_SubSystem.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace dogb
{
void
Inspector::onStart()
{
    m_title = "Inspector";
}

void
Inspector::onGUI(const UT::Timestep&)
{
    UT::Window *window = imguiContext()->window();
    if (window == nullptr)
    {
        UT_LOG_ERROR("IMGUI context is not attached to a window.");
        return;
    }
    UT::Engine& engine = UT::Engine::get();
    GS::SubSystem* gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();

    auto scene = gs_system->m_activeScene;

    EditorGUI(scene->m_entityManager, scene->m_selectedEntity);
}

} // namespace dogb