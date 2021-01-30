//
// Created by matt on 2020-07-11.
//

#include "GR_DesktopWindow.h"

#include "GR_Renderer.h"
#include <CE/CE_Input.h>

#include <UT/UT_Application.h>
#include <UT/UT_Logger.h>
#include <UT/UT_Profiler.h>

#include <glad/glad.h>

namespace dogb::GR
{
static bool theWindowInitialized = false;

namespace WindowEvent
{
struct DB_GR_API WindowResizeEvent : public detail::Event
{
    WindowResizeEvent(I32 _width, I32 _height)
        : detail::Event(detail::Type::kResizeEvent)
        , width(_width)
        , height(_height)
    {
    }

    I32 width;
    I32 height;
};

struct DB_GR_API WindowFocusEvent : public detail::Event
{
    explicit WindowFocusEvent(bool gained)
        : detail::Event(detail::Type::kFocusEvent), gainedFocus(gained)
    {
    }

    bool gainedFocus;
};

struct DB_GR_API WindowMovedEvent : public detail::Event
{
    WindowMovedEvent(I32 _width, I32 _height)
        : detail::Event(detail::Type::kWindowMovedEvent)
        , width(_width)
        , height(_height)
    {
    }

    int width;
    int height;
};

struct DB_GR_API WindowKeyEvent : public detail::Event
{
    WindowKeyEvent(const CE::Input::KeyEvent& ev)
        : detail::Event(detail::Type::kKeyEvent), event(ev)
    {
    }
    CE::Input::KeyEvent event;
};

struct DB_GR_API MouseMovedEvent : public detail::Event
{
    MouseMovedEvent(float _x, float _y)
        : detail::Event(detail::Type::kMouseMovedEvent), x(_x), y(_y)
    {
    }

    float x{};
    float y{};
};

struct DB_GR_API MouseScrolledEvent : public detail::Event
{
    MouseScrolledEvent(float x, float y)
        : detail::Event(detail::Type::kMouseScrolledEvent)
        , xOffset(x)
        , yOffset(y)
    {
    }

    float xOffset{};
    float yOffset{};
};

struct DB_GR_API MouseButtonEvent : public detail::Event
{
    MouseButtonEvent(
            CE::Input::MouseButtonType _button,
            CE::Input::ActionType _action)
        : detail::Event(detail::Type::kMouseButtonEvent)
        , button(_button)
        , action(_action)
    {
    }

    CE::Input::MouseButtonType button{};
    CE::Input::ActionType action{};
};
} // namespace WindowEvent

static void
grErrorCallback(int error, const char *description)
{
    UT_LOG_ERROR("GLFW ERROR ({}): {}", error, description);
}

static void
grGlfwWindowSizeCallback(GLFWwindow *window, int width, int height)
{
    auto desk_win =
            reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));
    desk_win->addEvent(
            std::make_unique<WindowEvent::WindowResizeEvent>(width, height));
}

static void
grGlfwWindowCloseCallback(GLFWwindow *window)
{
    auto desk_win =
            reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));
    // We cheat here a little in that we dont use a different object type as we
    // just need to pass the event type (there is no extra data required).
    desk_win->addEvent(std::make_unique<WindowEvent::detail::Event>(
            WindowEvent::detail::kCloseEvent));
}

static void
grGlfwScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    auto desk_win =
            reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));
    desk_win->addEvent(std::make_unique<WindowEvent::MouseScrolledEvent>(
            xOffset, yOffset));
}

static void
grGlfwKeyCallback(GLFWwindow *window, int key, int, int action, int modifier)
{
    using namespace CE::Input;

    auto desk_win = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));

    CE::Input::KeyEvent key_event{static_cast<CE::Input::KeyType>(key)};
    switch (action)
    {
    case GLFW_PRESS:
        key_event.m_action = ActionType::PRESS;
        break;
    case GLFW_RELEASE:
        key_event.m_action = ActionType::RELEASE;
        break;
    case GLFW_REPEAT:
        key_event.m_action = ActionType::REPEAT;
        break;
    default:
        UT_LOG_ERROR("Unkown key action.");
        break;
    }
    switch (modifier)
    {
    case GLFW_MOD_SHIFT:
        key_event.m_modifier = ModifierKeyType::MODIFIER_SHIFT;
        break;
    case GLFW_MOD_CONTROL:
        key_event.m_modifier = ModifierKeyType::MODIFIER_CTRL;
        break;
    case GLFW_MOD_ALT:
        key_event.m_modifier = ModifierKeyType::MODIFIER_ALT;
        break;
    case GLFW_MOD_SUPER:
        key_event.m_modifier = ModifierKeyType::MODIFIER_SUPER;
        break;
    default:
        key_event.m_modifier = ModifierKeyType::MODIFIER_NONE;
        break;
    }

    desk_win->addEvent(
            std::make_unique<WindowEvent::WindowKeyEvent>(key_event));
}

static void
grGlfwMouseButtonCallback(GLFWwindow *window, int button, int action, int)
{
    using namespace CE::Input;

    auto desk_win = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));
    ActionType _action = ActionType::ACTION_NONE;
    switch (action)
    {
    case GLFW_PRESS:
        _action = ActionType::PRESS;
        break;
    case GLFW_RELEASE:
        _action = ActionType::RELEASE;
        break;
    default:
        UT_LOG_ERROR("Unknown button action.");
        break;
    }
    desk_win->addEvent(std::make_unique<WindowEvent::MouseButtonEvent>(
            static_cast<MouseButtonType>(button), _action));
}

#if 0
static void
grGlfwCursorPosCallback(GLFWwindow *window, double xPos, double yPos)
{
    auto desk_win = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(window));
    desk_win->addEvent(std::make_unique<WindowEvent::MouseMovedEvent>(
            static_cast<float>(xPos), static_cast<float>(yPos)));
}
#endif

DesktopWindow::DesktopWindow(const UT::WindowProperties &props)
    : UT::Window(props)
{
}

void
DesktopWindow::shutdown()
{
    if (m_hasShutdown)
        return;
    m_hasShutdown = true;

    UT_PROFILE_FUNCTION();

    m_handle.reset();
}

void
DesktopWindow::use()
{
    glfwMakeContextCurrent(m_handle.get());
}

void
DesktopWindow::initialize()
{
    UT_PROFILE_FUNCTION();

    UT_LOG_DEBUG("Intializing Desktop Window");
    if (!theWindowInitialized)
    {
        glfwSetErrorCallback(grErrorCallback);

        if (!glfwInit())
            UT_LOG_ERROR("Failed to initialize GLFW");

        theWindowInitialized = true;

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, m_useTitlebar);

    m_handle = CE::WindowPtr(glfwCreateWindow(
            static_cast<int>(m_width), static_cast<int>(m_height),
            m_title.c_str(), nullptr, nullptr));

    glfwMakeContextCurrent(m_handle.get());

    // Initialize our renderer -- The renderer will ensure that its initialized
    // only once.
    Renderer::initWindow(this);

    glfwSetWindowUserPointer(m_handle.get(), this);
    setVSync(m_usingVSync);

    // GLFW callback
    glfwSetWindowSizeCallback(m_handle.get(), grGlfwWindowSizeCallback);
    glfwSetWindowCloseCallback(m_handle.get(), grGlfwWindowCloseCallback);
    glfwSetScrollCallback(m_handle.get(), grGlfwScrollCallback);
    glfwSetKeyCallback(m_handle.get(), grGlfwKeyCallback);
    glfwSetMouseButtonCallback(m_handle.get(), grGlfwMouseButtonCallback);
#if 0
    glfwSetCursorPosCallback(m_handle.get(), grGlfwCursorPosCallback);
#endif

    // Set win size
    glfwSetWindowSizeLimits(
            m_handle.get(), static_cast<int>(m_minWidth),
            static_cast<int>(m_minHeight), GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void
DesktopWindow::setVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    m_usingVSync = enabled;
}

void
DesktopWindow::onResize(I32 width, I32 height)
{
    if (width == 0 || height == 0)
    {
        m_minimized = true;
        return;
    }
    m_minimized = false;

    m_width = static_cast<U32>(width);
    m_height = static_cast<U32>(height);

    m_resizeSignal(width, height);

    Renderer::setViewport(0, 0, width, height);
}

void
DesktopWindow::onMouseScrolled(float x_offset, float y_offset)
{
    m_scrolledSignal(x_offset, y_offset);
}

void
DesktopWindow::close()
{
    UT_LOG_DEBUG("Close Window");
    m_open = false;

    m_closeSignal();

    UT::AppInterface::mainApp()->quitApp();
}

void DesktopWindow::update(UT::Timestep)
{
    UT_PROFILE_FUNCTION();

    glfwPollEvents();
    Renderer::swapBuffers(this);
}

void
DesktopWindow::flushEvents()
{
    while (!m_events.empty())
    {
        using namespace WindowEvent;
        using namespace WindowEvent::detail;

        auto &&e = m_events.front();
        if (e->m_type == kResizeEvent)
        {
            WindowResizeEvent *ev =
                    reinterpret_cast<WindowResizeEvent *>(e.get());

            onResize(ev->width, ev->height);
        }
        else if (e->m_type == kCloseEvent)
        {
            // We dont need to cast this object since its just used as a tag
            close();
        }
        else if (e->m_type == kMouseScrolledEvent)
        {
            MouseScrolledEvent *ev =
                    reinterpret_cast<MouseScrolledEvent *>(e.get());
            onMouseScrolled(ev->xOffset, ev->yOffset);
        }
        else if (e->m_type == kKeyEvent)
        {
            WindowKeyEvent* ev = reinterpret_cast<WindowKeyEvent*>(e.get());
            m_hotkeyManager.handle(ev->event);
        }
        else if (e->m_type == kMouseButtonEvent)
        {
            MouseButtonEvent* ev=
                reinterpret_cast<MouseButtonEvent*>(e.get());
            onMouseButton(ev->button, ev->action);
        }
        else
        {
            UT_LOG_ERROR("Event type not handled...");
        }
        m_events.pop();
    }
}

void
DesktopWindow::addEvent(std::unique_ptr<WindowEvent::detail::Event> e)
{
    m_events.emplace(std::move(e));
}
} // namespace dogb::GR