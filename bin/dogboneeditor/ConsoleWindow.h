//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_CONSOLEWINDOW_H
#define DOGBONE_CONSOLEWINDOW_H

#include <IMGUI/IMGUI_Window.h>

#include <UT/UT_Logger.h>

#include <list>

namespace dogb
{
class ConsoleWindow : public IMGUI::Window
{
    RTTR_ENABLE()

public:
    virtual void onStart() override;
    virtual void onGUI() override;
    virtual void onDestroy() override;

private:
#if 0
    void onLog(const std::string &log);
#endif

    UT::Logger::callback_id_t m_listenerID;

    std::vector<std::string> m_logs;
    unsigned m_max_logs;
};
} // namespace dogb

#endif // DOGBONE_CONSOLEWINDOW_H
