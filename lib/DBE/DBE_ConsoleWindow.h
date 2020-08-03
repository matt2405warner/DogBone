//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_DBE_CONSOLEWINDOW_H
#define DOGBONE_DBE_CONSOLEWINDOW_H

#include <IMGUI/IMGUI_Window.h>

#include <UT/UT_Logger.h>

#include <vector>
#include <string>

namespace dogb::DBE
{
class ConsoleWindow : public IMGUI::Window
{
public:
    void onStart() override;
    void onGUI(const UT::Timestep& ts) override;

private:
    std::vector<std::string> m_logs;
    unsigned m_max_logs;
};

} // namespace dogb

#endif // DOGBONE_DBE_CONSOLEWINDOW_H
