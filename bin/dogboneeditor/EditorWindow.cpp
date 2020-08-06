//
// Created by matt on 2020-07-11.
//

#include "EditorWindow.h"

#include "GameWindow.h"
#include "ProjectWindow.h"

#include <DBE/DBE_ConsoleWindow.h>
#include <DBE/DBE_EditorIMGUIContext.h>
#include <DBE/DBE_HierarchyWindow.h>
#include <DBE/DBE_Inspector.h>
#include <DBE/DBE_SceneWindow.h>

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_CameraComponent.h>
#include <GS/GS_CameraController.h>
#include <GS/GS_Mesh2DComponent.h>
#include <GS/GS_World.h>

#include <UT/UT_Assert.h>

namespace dogb
{
//#define USE_3D

void
EditorWindow::initialize()
{
    GR::DesktopWindow::initialize();

    GR::Framebuffer::Specification spec;
    spec.m_width = m_width;
    spec.m_height = m_height;

    GS::World &world = GS::World::instance();
    world.mainCamera()->m_activeTexture = GR::Framebuffer::create(spec);
    world.mainCamera()->setViewportSize(m_width, m_height);

    auto imgui_ctx =
            addContext<IMGUI::SubSystem, DBE::EditorIMGUIContext>(this);
    UT_ASSERT(imgui_ctx != nullptr);

    // Add all of the standard editor GUI windows
    DBE::Inspector *inspector = imgui_ctx->createGUIWindow<DBE::Inspector>();
    inspector->show();
    ProjectWindow *proj_window = imgui_ctx->createGUIWindow<ProjectWindow>();
    proj_window->show();

    DBE::HierarchyWindow *hierarchy =
            imgui_ctx->createGUIWindow<DBE::HierarchyWindow>();
    hierarchy->show();

    DBE::SceneWindow *scene_win =
            imgui_ctx->createGUIWindow<DBE::SceneWindow>();
    scene_win->show();

    GameWindow *game_win = imgui_ctx->createGUIWindow<GameWindow>();
    game_win->show();

    DBE::ConsoleWindow *console_win =
            imgui_ctx->createGUIWindow<DBE::ConsoleWindow>();
    console_win->show();

#ifdef USE_3D
    auto ctx = addContext<GS::SubSystem, TestContext_3D>(scene_win);
#endif

#ifdef USE_3D
    proj_window->m_ctx = ctx.get();
#endif

    auto ent = world.createEntity();
    ent.addComponent<GS::Mesh2DComponent>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
    // m_testEntity.addComponent<TestComponent>(5);
    world.createEntity();
    world.createEntity();
    auto cam_ent = world.createEntity();
    GS::CameraComponent &cam_comp = cam_ent.addComponent<GS::CameraComponent>();
    cam_comp.m_camera = world.mainCamera();
    cam_ent.addComponent<GS::CameraController>();

    world.m_selectedEntity = ent;
}

} // namespace dogb