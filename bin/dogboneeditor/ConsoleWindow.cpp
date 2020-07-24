//
// Created by matt on 2020-07-11.
//

#include "ConsoleWindow.h"

#include <imgui.h>

#include <iostream>

namespace dogb
{
void
ConsoleWindow::onStart()
{
    m_title = "Console";

    m_max_logs = 100;

#if 0
    UT::Logger &logger = UT::Logger::get();
    m_listenerID = logger.registerCallback(
        [=](const std::string& log)
        {
            onLog(log);
        },
        [=]()
        {
            std::cout << std::flush;
        });
#endif
    UT_LOG_TRACE("Console onStart");
}

void
ConsoleWindow::onDestroy()
{
#if 0
    UT::Logger& logger = UT::Logger::get();

    logger.removeCallback(m_listenerID);
#endif
}

void
ConsoleWindow::onGUI()
{
    if (ImGui::Button("Clear"))
        m_logs.clear();

    ImGui::Separator();

    ImGui::BeginChild("scrolling");

    UT::Logger &logger = UT::Logger::get();
    auto &&logs = logger.logs();
    for (auto&& log : logs)
        printf("Log: %s", log.c_str());
    m_logs.insert(m_logs.end(), logs.begin(), logs.end());
    logger.clear();

    for (const std::string &l : m_logs)
        ImGui::TextUnformatted(l.c_str());

    ImGui::EndChild();
}

#if 0
void
ConsoleWindow::onLog(const std::string &log)
{
    if (m_logs.size() + 1 > m_max_logs)
        m_logs.pop_front();
    m_logs.emplace_back(log);
}
#endif

} // namespace dogdb