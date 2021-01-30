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

void
EditorWindow::onMouseScrolled(float x_offset, float y_offset)
{
    DesktopWindow::onMouseScrolled(x_offset, y_offset);

    GS::Editor::camera().onMouseScroll(x_offset, y_offset);
}
void
EditorWindow::update(UT::Timestep ts)
{
    DesktopWindow::update(ts);

    GS::Editor::instance().onUpdate(ts);
}

void
EditorWindow::onMouseButton(
        CE::Input::MouseButtonType button,
        CE::Input::ActionType action)
{
    if (button != CE::Input::MouseButtonType::MOUSE_BUTTON_LEFT)
        return;
    else if (action != CE::Input::ActionType::PRESS)
        return;

    auto[mx, my] = ImGui::GetMousePos();
    //UT_LOG_WARN("Mouse Pos = {0}, {1}", mx, my);
    mx -= m_sceneWindow->m_viewportBounds[0].x;
    my -= m_sceneWindow->m_viewportBounds[0].y;
    glm::vec2 vsize = m_sceneWindow->m_viewportBounds[1] - m_sceneWindow->m_viewportBounds[0];
    my = vsize.y - my;
    int mouse_x = (int)mx;
    int mouse_y = (int)my;
    if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < (int)vsize.x && mouse_y < (int)vsize.y)
    {
        auto frame_buffer = GS::Editor::instance().framebuffer();
        //UT_LOG_WARN("Mouse = {0}, {1}", mouse_x, mouse_y);
        frame_buffer->bind();
        int px = frame_buffer->readPixel(1, mouse_x, mouse_y);
        frame_buffer->unbind();
#if 0
        if (px != -1)
        {
            GS::World::instance().m_selectedEntity = GS::Entity(
                    static_cast<entt::entity>(px),
                    GS::World::instance().m_activeScene->m_entityManager);
        }
#endif
        UT_LOG_WARN("Pixel: {0}", px);
    }
}

} // namespace dogb