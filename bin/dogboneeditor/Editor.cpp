//
// Created by matt on 2020-07-11.
//

#include "Editor.h"

#include "EditorWindow.h"

#include "ProjectWindow.h"

#include <GR/GR_RenderBuffer.h>
#include <GR/GR_Renderer.h>
#include <GR/GR_Shader.h>
#include <GR/GR_VertexArray.h>
#include <GR/GR_SubSystem.h>

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_SubSystem.h>
#include <GS/GS_Scene.h>

#include <CE/CE_Input.h>

#include <UT/UT_Logger.h>

#include <UT/UT_Assert.h>
#include <unistd.h>

namespace dogb
{
std::unique_ptr<dogb::UT::Application>
EditorInterface::create()
{
    return std::make_unique<Editor>("DogBone Editor");
}

int
Editor::initialize(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    using namespace dogb::UT;

    auto sink = std::make_shared<UT::logger_sink_mt>();
    auto log = std::make_shared<spdlog::logger>("Editor", sink);

#ifndef NDEBUG
    log->set_level(spdlog::level::debug);
#else
    log->set_level(spdlog::level::err);
#endif

    UT::Logger &logger = UT::Logger::get();
    logger.setLog(std::move(log));

    WindowProperties props("Hello World", 1080, 920, 800, 600, true, true);
    createWindow<EditorWindow>(props);

    GR::Renderer::init();

    return 0;
}

int
Editor::exec()
{
    Application::exec();

    entt::registry registry;
    auto entity = registry.create();
    (void)entity;

    UT::Engine& engine = UT::Engine::get();

    GR::ShaderLibrary shader_library;

    EditorWindow *window = dynamic_cast<EditorWindow *>(m_mainWindow.get());
    GR::Renderer::use(window);

    float last_time = 0;

    GR::SubSystem* gr_system = engine.getOrCreateSubSystem<GR::SubSystem>();
    gr_system->init();

    GS::SubSystem* gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();
    gs_system->init();

    IMGUI::SubSystem* imgui_system = engine.getOrCreateSubSystem<IMGUI::SubSystem>();
    imgui_system->init();

    while (isRunning())
    {
        using namespace CE::Input;

        float time = static_cast<float>(glfwGetTime());
        UT::Timestep timestep = time - last_time;
        last_time = time;

        if (!window->isMinimized())
        {
            m_mainWindow->preUpdate();

            gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();
            gs_system->update(timestep);

            imgui_system = engine.getOrCreateSubSystem<IMGUI::SubSystem>();
            imgui_system->update(timestep);

            m_mainWindow->postUpdate();

            gr_system = engine.getOrCreateSubSystem<GR::SubSystem>();
            gr_system->update(timestep);
        }

        window->update(timestep);

        window->flushEvents();

        // UT_LOG_INFO("Delta Time: {} ({}ms)", timestep.seconds(),
        // timestep.milliseconds());
    }

    gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();
    gs_system->shutdown();

    imgui_system = engine.getOrCreateSubSystem<IMGUI::SubSystem>();
    imgui_system->shutdown();

    gr_system = engine.getOrCreateSubSystem<GR::SubSystem>();
    gr_system->shutdown();

    return 0;
}
int
Editor::cleanup()
{
    int rcode = Application::cleanup();
    if (rcode != 0)
        return rcode;

    // Make sure to terminate glfw now that everything has been cleaned up.
    glfwTerminate();

    return 0;
}
} // namespace dogb