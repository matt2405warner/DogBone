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
#include <DBE/DBE_Editor.h>

#include <IMGUI/IMGUI_SubSystem.h>
#include <IMGUI/IMGUI_FileBrowser.h>

#include <GS/GS_CameraComponent.h>
#include <GS/GS_CameraController.h>
#include <GS/GS_Mesh2DComponent.h>
#include <GS/GS_MeshComponent.h>
#include <GS/GS_World.h>
#include <GS/GS_Material.h>
#include <GS/GS_SceneSerializer.h>

#include <UT/UT_Assert.h>
#include <UT/UT_FileDialog.h>

DBE_REGISTER_MENU("Component/Add Component", [](){})

namespace dogb
{
void
EditorWindow::initialize()
{
    GR::DesktopWindow::initialize();

    DBE::Editor::addMenuCallback("File/New Scene...", [this]() {
        GS::World &world = GS::World::instance();
        world.m_activeScene = std::make_shared<GS::Scene>();
        // This is only a hack until there is a proper editor camera and
        // game camera.
        world.m_mainCamera = nullptr;
        world.m_activeScene->onViewportResize(
                static_cast<uint32_t>(m_sceneWindow->m_viewportSize.x),
                static_cast<uint32_t>(m_sceneWindow->m_viewportSize.y));
    });

    DBE::Editor::addMenuCallback("File/Load...", [this]() {
        std::optional<std::string> path = UT::openFile(
                "Load Scene", "DogBone Scene | *.dbscene");
        if (path)
        {
            UT_LOG_INFO("Load Path: '{}'", path.value());
            GS::World &world = GS::World::instance();
            world.m_activeScene = std::make_shared<GS::Scene>();
            world.m_mainCamera = nullptr;
            world.m_activeScene->onViewportResize(
                    static_cast<uint32_t>(m_sceneWindow->m_viewportSize.x),
                    static_cast<uint32_t>(m_sceneWindow->m_viewportSize.y));

            GS::SceneSerializer serializer(world.m_activeScene);
            serializer.deserialize(path.value());
        }
    });

    DBE::Editor::addMenuCallback("File/Save...", []() {
        std::optional<std::string> path = UT::saveFile(
                "Save Scene", "DogBone Scene | *.dbscene");
        if (path)
        {
            GS::World &world = GS::World::instance();

            GS::SceneSerializer serializer(world.m_activeScene);
            serializer.serialize(path.value());
        }
    });

    DBE::Editor::addMenuCallback("Entity/Create Empty", []() {
        GS::World &world = GS::World::instance();
        GS::Entity e = world.createEntity();
        if (world.m_selectedEntity)
        {
            world.m_selectedEntity.addChildEntity(e);
        }
        else
        {
            world.m_selectedEntity = e;
        }
    });

    DBE::Editor::addMenuCallback("Help/About", [](){

    });

    auto imgui_ctx = std::make_shared<DBE::EditorIMGUIContext>(this);
    IMGUI::SubSystem::instance().attach(imgui_ctx);
    UT_ASSERT(imgui_ctx != nullptr);

    // Add all of the standard editor GUI windows
    DBE::Inspector *inspector = imgui_ctx->createGUIWindow<DBE::Inspector>();
    inspector->show();
    ProjectWindow *proj_window = imgui_ctx->createGUIWindow<ProjectWindow>();
    proj_window->show();

    DBE::HierarchyWindow *hierarchy =
            imgui_ctx->createGUIWindow<DBE::HierarchyWindow>();
    hierarchy->show();

    m_sceneWindow =
            imgui_ctx->createGUIWindow<DBE::SceneWindow>();
    m_sceneWindow->show();

    GameWindow *game_win = imgui_ctx->createGUIWindow<GameWindow>();
    game_win->show();

    DBE::ConsoleWindow *console_win =
            imgui_ctx->createGUIWindow<DBE::ConsoleWindow>();
    console_win->show();

    initializeHotKeys();
#if 0
    GS::World &world = GS::World::instance();

    GS::SceneSerializer serializer(world.m_activeScene);
    serializer.serialize("../assets/scenes/test.dbscene");
    serializer.deserialize("../assets/scenes/test.dbscene");
#endif
}
void
EditorWindow::initializeHotKeys()
{
    using namespace CE::Input;

    CE::HotKeyConfiguration global_configuration;
    global_configuration.setHotKey(
            HotKey{.m_key = KeyType::KEY_Q},
            std::bind(
                    &DBE::SceneWindow::onUpdateKeys, m_sceneWindow,
                    std::placeholders::_1));
    global_configuration.setHotKey(
            HotKey{.m_key = KeyType::KEY_E},
            std::bind(
                    &DBE::SceneWindow::onUpdateKeys, m_sceneWindow,
                    std::placeholders::_1));
    global_configuration.setHotKey(
            HotKey{.m_key = KeyType::KEY_R},
            std::bind(
                    &DBE::SceneWindow::onUpdateKeys, m_sceneWindow,
                    std::placeholders::_1));
    global_configuration.setHotKey(
            HotKey{.m_key = KeyType::KEY_T},
            std::bind(
                    &DBE::SceneWindow::onUpdateKeys, m_sceneWindow,
                    std::placeholders::_1));

    m_hotkeyManager.setGlobalConfiguration(global_configuration);
}

} // namespace dogb