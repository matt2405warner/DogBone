//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_IMGUI_WINDOW_H
#define DOGBONE_IMGUI_WINDOW_H

#include "IMGUI_API.h"

#include <UT/UT_Timestep.h>

#include <optional>
#include <string>

#include <imgui.h>

#include <rttr/type>

namespace dogb::IMGUI
{
class SubSystemContext;

class DB_IMGUI_API Window
{
    friend class IMGUI::SubSystemContext;
public:
    enum DockDirection
    {
        DockNone,
        DockLeft,
        DockRight,
        DockUp,
        DockDown
    };

    explicit Window()
        : m_isOpen(false), m_dockDirection(DockNone), m_initializedDock(false)
    {
    }
    virtual ~Window() = default;

    struct DB_IMGUI_API Style
    {
        ImVec2 m_padding{10, 10};
    };

    // Called whenever this window is enabled
    virtual void onStart()
    {
        rttr::type t = rttr::type::get(*this);
        if (t.is_valid())
            m_title = std::string(t.get_name().begin(), t.get_name().length());
        else
            m_title = "##ERROR##";
    }

    // Called when the window is destroyed
    virtual void onDestroy() {}

    void onPreGUI() const
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_style.m_padding);
    }
    // Called when repainting
    virtual void onGUI(const UT::Timestep& ts) = 0;
    void onPostGUI()
    {
        ImGui::PopStyleVar();
    }

    void show() { m_isOpen = true; }
    [[nodiscard]] bool isOpen() const { return m_isOpen; }

    const std::string &title() { return m_title; }

protected:
    std::string m_title;

    // This tells the IMGUI context whether or not to show the window on
    // the contexts update
    bool m_isOpen;
    // This tells the IMGUI context whether or not we are finished with this
    // window and can destroy the instance after updating
    bool m_finished{};

    [[nodiscard]] IMGUI::SubSystemContext *imguiContext() const;

    Style m_style;

    DockDirection m_dockDirection;
private:

    bool m_initializedDock;
    IMGUI::SubSystemContext *m_context{};
};
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_WINDOW_H
