//
// Created by matt on 2020-07-11.
//

#include "DBE_ConsoleWindow.h"

#include <imgui.h>

namespace dogb::DBE
{
void
ConsoleWindow::onStart()
{
    m_title = "Console";

    m_max_logs = 100;

    m_dockDirection = DockDown;
}

void
ConsoleWindow::onGUI(const UT::Timestep&)
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

} // namespace dogdb