//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_DESKTOPWINDOW_H
#define DOGBONE_GR_DESKTOPWINDOW_H

#include "GR_API.h"

#include <CE/CE_GLFWHelpers.h>
#include <CE/CE_Input.h>

#include <UT/UT_EventDispatcher.h>
#include <UT/UT_Window.h>

#include <boost/signals2.hpp>

#include <memory>
#include <queue>
#include <utility>

namespace dogb::GR
{
#if 1
namespace WindowEvent::detail
{
enum Type
{
    kResizeEvent = 0,
    kCloseEvent,
    kFocusEvent,
    kWindowMovedEvent,
    kKeyEvent,
    kMouseMovedEvent,
    kMouseScrolledEvent,
    kMouseButtonEvent
};

struct DB_GR_API Event
{
    explicit Event(Type type) : m_type(type) {}
    const Type m_type;
};
}

#endif

class DB_GR_API DesktopWindow : public UT::Window
{
public:
    using ResizeSignalType = boost::signals2::signal<void(I32, I32)>;
    using CloseSignalType = boost::signals2::signal<void()>;
    using MouseScrolledSignalType = boost::signals2::signal<void(float, float)>;

    explicit DesktopWindow(const UT::WindowProperties &props);

    void use() override;

    void preUpdate() override {}
    void update(UT::Timestep ts) override;
    void postUpdate() override {}

    void shutdown() override;

    void setVSync(bool enabled) override;

    void initialize() override;

    void close() override;

    void flushEvents();

    void *nativeWindow() override { return m_handle.get(); }

    [[nodiscard]] bool isMinimized() const { return m_minimized; }

    void addEvent(std::unique_ptr<WindowEvent::detail::Event> e);

    ResizeSignalType m_resizeSignal;
    CloseSignalType m_closeSignal;
    MouseScrolledSignalType m_scrolledSignal;

protected:
    virtual void onResize(I32 width, I32 height);
    virtual void onMouseScrolled(float x_offset, float y_offset);

private:
    [[nodiscard]] CE::WindowType *handle() { return m_handle.get(); }

    CE::WindowPtr m_handle;

    bool m_minimized = false;
    bool m_hasShutdown = false;
    std::queue<std::unique_ptr<WindowEvent::detail::Event>> m_events;
};

namespace WindowEvent
{
} // namespace WindowEvent

} // namespace dogb::GR

#endif // DOGBONE_GR_DESKTOPWINDOW_H
