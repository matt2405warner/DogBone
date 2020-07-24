//
// Created by matt on 2020-07-11.
//

#include "EditorWindow.h"

#include <IMGUI/IMGUI_SubSystem.h>
#include <IMGUI/IMGUI_SubSystemContext.h>

#include <GS/GS_SubSystem.h>
#include <GS/GS_TransformComponent.h>

#include <GR/GR_SubSystem.h>

#include <UT/UT_Assert.h>

#include "ConsoleWindow.h"
#include "GameWindow.h"
#include "HierarchyWindow.h"
#include "Inspector.h"
#include "ProjectWindow.h"
#include "SceneWindow.h"
#include "TestContext_2D.h"
#include "TestContext_3D.h"

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

    m_framebuffer = GR::Framebuffer::create(spec);

    auto imgui_ctx =
            addContext<IMGUI::SubSystem, IMGUI::SubSystemContext>(this);
    UT_ASSERT(imgui_ctx != nullptr);

    // Add all of the standard editor GUI windows
    Inspector *inspector = imgui_ctx->createGUIWindow<Inspector>();
    // imgui_ctx->dockGUIWindowRight(*inspector);
    inspector->show();
    ProjectWindow *proj_window = imgui_ctx->createGUIWindow<ProjectWindow>();

    // imgui_ctx->dockGUIWindowLeft(*project);
    proj_window->show();

    // std::optional<rttr::type> t(rttr::type::get<ProjectWindow>());
    HierarchyWindow *hierarchy = imgui_ctx->createGUIWindow<HierarchyWindow>();
    hierarchy->show();

    SceneWindow *scene_win = imgui_ctx->createGUIWindow<SceneWindow>(this);
    scene_win->m_framebuffer = m_framebuffer;
    scene_win->m_cameraController.attach(*this);
    // imgui_ctx->dockGUIWindowUp(*scene_win);
    scene_win->show();

    // std::optional<rttr::type> t(rttr::type::get<SceneWindow>());
    GameWindow *game_win = imgui_ctx->createGUIWindow<GameWindow>();
    game_win->show();

    ConsoleWindow *console_win = imgui_ctx->createGUIWindow<ConsoleWindow>();
    // imgui_ctx->dockGUIWindowDown(*console_win);
    console_win->show();

#ifndef USE_3D
    auto ctx = addContext<GS::SubSystem, TestContext_2D>(scene_win);
#else
    auto ctx = addContext<GS::SubSystem, TestContext_3D>(scene_win);
#endif

#ifdef USE_3D
    proj_window->m_ctx = ctx.get();
#else
    proj_window->m_ctx2D = ctx.get();
#endif

    ctx->m_framebuffer = m_framebuffer;
}
void
EditorWindow::shutdown()
{
    DesktopWindow::shutdown();

    m_framebuffer = nullptr;
}

} // namespace dogb