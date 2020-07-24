//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H
#define DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H

#include "IMGUI_API.h"

#include "IMGUI_Window.h"

#include <UT/UT_SubSystemContext.h>
#include <UT/UT_Timestep.h>

#include <imgui.h>
#include <rttr/type.h>

#include <memory>
#include <optional>
#include <vector>

class ImGuiContext;

namespace dogb::UT
{
class Window;
}

namespace dogb::IMGUI
{
class IMGUI_API SubSystemContext : public UT::SubSystemContext
{
    friend class SubSystem;

public:
    explicit SubSystemContext(UT::Window* window);
    ~SubSystemContext() override;
    void onAttach();
    void onShutdown();

    void update(UT::Timestep timestep) override;

    void setWindow(UT::Window *window) { m_window = window; }
    [[nodiscard]] UT::Window *window() const { return m_window; }

    template <typename T, typename... Args>
    T *createGUIWindow(Args&&... args)
    {
        static_assert(
                std::is_base_of_v<IMGUI::Window, T>,
                "Cannot create GUI window if it does not derive from IMGUI::Window");
#if 0
        IMGUI::Window::DockDirection dock_dir;
        // Find who we are docking to and set it to the same dock space
        if (dock_to)
        {
            const rttr::type t = dock_to.value();

            // Search all windows
            for (auto iter = m_windows.begin(); iter != m_windows.end(); ++iter)
            {
                const std::unique_ptr<IMGUI::Window> &ptr = *iter;
                const rttr::type win_t = rttr::type::get(*ptr);

                if (t == win_t)
                {
                    dock_dir = (*iter)->m_dockDirection;
                    break;
                }
            }
        }
#endif

        // Add the new window and set the dock direction if any
        std::unique_ptr<IMGUI::Window> &win =
                m_windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        win->m_context = this;
#if 0
        win->m_dockDirection = dock_dir;
#endif

        win->onStart();

        return reinterpret_cast<T *>(win.get());
    }

    template <typename T>
    T *getGUIWindow()
    {
        static_assert(
                std::is_base_of_v<IMGUI::Window, T>,
                "Cannot get GUI window if it does not derive from IMGUI::Window");

        const rttr::type t = rttr::type::get<T>();
        for (auto & ptr : m_windows)
        {
            const rttr::type win_t = rttr::type::get(*ptr);

            if (t == win_t)
                return ptr.get();
        }

        return nullptr;
    }

#if 0
    void dockGUIWindowRight(IMGUI::Window &win);
    void dockGUIWindowLeft(IMGUI::Window &win);
    void dockGUIWindowUp(IMGUI::Window &win);
    void dockGUIWindowDown(IMGUI::Window &win);
#endif

protected:
    static void initStyle();

private:

#if 0
    // Translate a dock direction to an IMGUI id.
    ImGuiID getDockID(IMGUI::Window::DockDirection dir);
#endif
#if 0
    ImGuiID m_dockspaceID;
    ImGuiID m_dockLeftID;
    ImGuiID m_dockRightID;
    ImGuiID m_dockUpID;
    ImGuiID m_dockDownID;

    bool m_dockspaceInitialized;
#endif
    // Every window this context needs to draw
    std::vector<std::unique_ptr<IMGUI::Window>> m_windows;
    UT::Window *m_window{};
    ImGuiContext *m_imguiContext{};
};
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H
