//
// Created by matt on 2020-07-11.
//

#include "EditorWindow.h"

#include "ConsoleWindow.h"
#include "GameWindow.h"
#include "ProjectWindow.h"
#include "TestContext_3D.h"
#include "TestSpriteComponent.h"

#include <DBE/DBE_Inspector.h>
#include <DBE/DBE_SceneWindow.h>
#include <DBE/DBE_HierarchyWindow.h>

#include <IMGUI/IMGUI_SubSystem.h>
#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GS/GS_SubSystem.h>
#include <GS/GS_TransformComponent.h>
#include <GS/GS_World.h>

#include <GR/GR_SubSystem.h>

#include <UT/UT_Assert.h>

namespace dogb
{
//#define USE_3D

void
EditorWindow::initialize()
{
    GR::DesktopWindow::initialize();

    GR::Framebuffer::Specification spec;
    spec.m_width = static_cast<uint32_t>(m_width);
    spec.m_height = static_cast<uint32_t>(m_height);

    GS::World &world = GS::World::instance();
    world.mainCamera()->m_activeTexture = GR::Framebuffer::create(spec);

    auto imgui_ctx =
            addContext<IMGUI::SubSystem, IMGUI::SubSystemContext>(this);
    UT_ASSERT(imgui_ctx != nullptr);

    // Add all of the standard editor GUI windows
    DBE::Inspector *inspector = imgui_ctx->createGUIWindow<DBE::Inspector>();
    // imgui_ctx->dockGUIWindowRight(*inspector);
    inspector->show();
    ProjectWindow *proj_window = imgui_ctx->createGUIWindow<ProjectWindow>();

    // imgui_ctx->dockGUIWindowLeft(*project);
    proj_window->show();

    // std::optional<rttr::type> t(rttr::type::get<ProjectWindow>());
    DBE::HierarchyWindow *hierarchy = imgui_ctx->createGUIWindow<DBE::HierarchyWindow>();
    hierarchy->show();

    DBE::SceneWindow *scene_win =
            imgui_ctx->createGUIWindow<DBE::SceneWindow>(this);
    scene_win->m_cameraController.attach(*this);
    // imgui_ctx->dockGUIWindowUp(*scene_win);
    scene_win->show();

    // std::optional<rttr::type> t(rttr::type::get<SceneWindow>());
    GameWindow *game_win = imgui_ctx->createGUIWindow<GameWindow>();
    game_win->show();

    ConsoleWindow *console_win = imgui_ctx->createGUIWindow<ConsoleWindow>();
    // imgui_ctx->dockGUIWindowDown(*console_win);
    console_win->show();

#ifdef USE_3D
    auto ctx = addContext<GS::SubSystem, TestContext_3D>(scene_win);
#endif

#ifdef USE_3D
    proj_window->m_ctx = ctx.get();
#endif

    auto ent = world.createEntity();
    ent.addComponent<TestSpriteComponent>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
    // m_testEntity.addComponent<TestComponent>(5);
    world.createEntity();
    world.createEntity();

    world.m_selectedEntity = ent;
}
void
EditorWindow::shutdown()
{
    DesktopWindow::shutdown();
}

} // namespace dogb