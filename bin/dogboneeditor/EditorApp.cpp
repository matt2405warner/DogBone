//
// Created by matt on 2020-07-11.
//

#include "EditorApp.h"

#include "EditorWindow.h"

#include "ProjectWindow.h"

#include <GR/GR_RenderBuffer.h>
#include <GR/GR_Renderer.h>
#include <GR/GR_Shader.h>
#include <GR/GR_SubSystem.h>
#include <GR/GR_VertexArray.h>

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_Scene.h>
#include <GS/GS_SubSystem.h>
#include <GS/GS_Editor.h>

#include <CE/CE_Input.h>

#include <UT/UT_Logger.h>

#include <UT/UT_Assert.h>
#include <unistd.h>

namespace dogb
{
std::unique_ptr<dogb::UT::Application>
EditorInterface::create()
{
    return std::make_unique<EditorApp>("DogBone EditorApp");
}

int
EditorApp::initialize(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    using namespace dogb::UT;

    // NB: Make sure the editor is run as the editor and not runtime.
    GS::Editor::setEditor(true);

    auto sink = std::make_shared<UT::logger_sink_mt>();
    auto log = std::make_shared<spdlog::logger>("EditorApp", sink);

#ifndef NDEBUG
    log->set_level(spdlog::level::debug);
#else
    log->set_level(spdlog::level::err);
#endif

    UT::Logger &logger = UT::Logger::get();
    logger.setLog(std::move(log));

    GS::SubSystem &gs_system = GS::SubSystem::instance();
    gs_system.init();

    WindowProperties props("DogBone Editor", 1600, 900, 800, 600, true, true);
    createWindow<EditorWindow>(props);

    // Initialize our editor.
    GS::Editor::instance().initialize();

    GR::SubSystem &gr_system = GR::SubSystem::instance();
    gr_system.init();

    IMGUI::SubSystem &imgui_system = IMGUI::SubSystem::instance();
    imgui_system.init();

    return 0;
}

int
EditorApp::exec()
{
    Application::exec();

    EditorWindow *window = dynamic_cast<EditorWindow *>(m_mainWindow.get());
    GR::Renderer::use(window);

    float last_time = 0;

    GS::SubSystem &gs_system = GS::SubSystem::instance();
    GR::SubSystem &gr_system = GR::SubSystem::instance();
    IMGUI::SubSystem &imgui_system = IMGUI::SubSystem::instance();

    while (isRunning())
    {
        using namespace CE::Input;

        float time = static_cast<float>(glfwGetTime());
        UT::Timestep timestep = time - last_time;
        last_time = time;

        if (!window->isMinimized())
        {
            m_mainWindow->preUpdate();

            gs_system.update(timestep);

            imgui_system.update(timestep);

            m_mainWindow->postUpdate();

            gr_system.update(timestep);
        }

        window->update(timestep);

        window->flushEvents();

        // UT_LOG_INFO("Delta Time: {} ({}ms)", timestep.seconds(),
        // timestep.milliseconds());
    }

    gs_system.shutdown();

    imgui_system.shutdown();

    GS::Editor::instance().shutdown();

    gr_system.shutdown();

    return 0;
}
int
EditorApp::cleanup()
{
    int rcode = Application::cleanup();
    if (rcode != 0)
        return rcode;

    // Make sure to terminate glfw now that everything has been cleaned up.
    glfwTerminate();

    return 0;
}
} // namespace dogb