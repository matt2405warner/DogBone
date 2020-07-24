//
// Created by matt on 2020-07-11.
//

#include "UT_Application.h"

#include "UT_Profiler.h"

namespace dogb::UT
{
std::unique_ptr<Application> AppInterface::s_instance = nullptr;

int
AppInterface::run(int argc, char *argv[])
{
    // Create the application
    s_instance = create();

    int rcode = 0;
    {
        std::string startup_name = s_instance->m_name;
        startup_name.append("-startup.json");
        UT_PROFILE_BEGIN_SESSION("startup", startup_name);

        /*#ifndef NDEBUG
            spdlog::set_level(spdlog::level::debug);
        #else
            spdlog::set_level(spdlog::level::error);
        #endif*/

        // Initialize
        rcode = s_instance->initialize(argc, argv);
        if (rcode != 0)
        {
            UT_LOG_ERROR("Failed application initialization");
            return rcode;
        }

        UT_PROFILE_END_SESSION();
    }

    {
        std::string run_name = s_instance->m_name;
        run_name.append("-runtime.json");
        UT_PROFILE_BEGIN_SESSION("runtime", run_name);
        // Run
        rcode = s_instance->exec();
        if (rcode != 0)
        {
            UT_LOG_ERROR("Error running application");
            return rcode;
        }

        UT_PROFILE_END_SESSION();
    }

    {
        std::string shutdown_name = s_instance->m_name;
        shutdown_name.append("-shutdown.json");
        UT_PROFILE_BEGIN_SESSION("shutdown", shutdown_name);

        // Should already be false but just in case set it to false.
        s_instance->quitApp();

        // Cleanup
        rcode = s_instance->cleanup();

        UT_PROFILE_END_SESSION();
    }

    if (rcode != 0)
        UT_LOG_ERROR("Failed cleanup");
    return rcode;
}

int
Application::cleanup()
{
    UT_LOG_DEBUG("Cleanup application");

    m_mainWindow->shutdown();
    m_mainWindow.reset();

    return 0;
}
} // namespace dogb::UT