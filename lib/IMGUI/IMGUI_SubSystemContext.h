//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H
#define DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H

#include "IMGUI_API.h"

#include "IMGUI_Window.h"
#include "IMGUI_Modal.h"

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
class DB_IMGUI_API SubSystemContext : public UT::SubSystemContext
{
    friend class SubSystem;

public:
    explicit SubSystemContext(UT::Window* window);
    ~SubSystemContext() override;
    void onAttach();
    void onShutdown();

    void update(UT::Timestep timestep) override;
    virtual void onGUI() {}

    void setWindow(UT::Window *window) { m_window = window; }
    [[nodiscard]] UT::Window *window() const { return m_window; }

    template <typename T, typename... Args>
    T *createGUIWindow(Args&&... args)
    {
        static_assert(
                std::is_base_of_v<IMGUI::Window, T>,
                "Cannot create GUI window if it does not derive from IMGUI::Window");

        // Add the new window and set the dock direction if any
        std::unique_ptr<IMGUI::Window> &win =
                m_windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        win->m_context = this;

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
                return reinterpret_cast<T*>(ptr.get());
        }

        return nullptr;
    }

    Modal* addModal(std::unique_ptr<Modal> modal);

protected:
    static void initStyle();

private:
    void renderModals();

    // Translate a dock direction to an IMGUI id.
    ImGuiID getDockID(IMGUI::Window::DockDirection dir);
    ImGuiID m_dockspaceID;
    ImGuiID m_dockLeftID;
    ImGuiID m_dockRightID;
    ImGuiID m_dockUpID;
    ImGuiID m_dockDownID;

    bool m_dockspaceInitialized;
    // Every window this context needs to draw
    std::vector<std::unique_ptr<IMGUI::Window>> m_windows;
    UT::Window *m_window{};
    ImGuiContext *m_imguiContext{};

    std::vector<std::unique_ptr<Modal>> m_modals;
};
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_SUBSYSTEMCONTEXT_H
