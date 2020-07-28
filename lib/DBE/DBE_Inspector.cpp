//
// Created by matt on 2020-07-27.
//

#include "DBE_Inspector.h"

#include "DBE_Editor.h"

#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GS/GS_SubSystem.h>
#include <GS/GS_World.h>

#include <UT/UT_Logger.h>
#include <UT/UT_Window.h>

namespace dogb::DBE
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

    GS::World& world = GS::World::instance();
    DBE::Editor::drawEntity(world.m_entityManager, world.m_selectedEntity);
}
}