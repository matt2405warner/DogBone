//
// Created by matt on 2020-07-11.
//

#include "UT_Window.h"

namespace dogb::UT
{
static U32 theIDs = 0;

WindowProperties::WindowProperties(
        const String &title,
        I32 width,
        I32 height,
        I32 min_width,
        I32 min_height,
        bool vsync,
        bool has_title_bar)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_minWidth(min_width)
    , m_minHeight(min_height)
    , m_hasTitlebar(has_title_bar)

    , m_vsync(vsync)
{
}

Window::Window(const WindowProperties &props)
    : m_title(props.m_title)
    , m_width(props.m_width)
    , m_height(props.m_height)
    , m_minWidth(props.m_minWidth)
    , m_minHeight(props.m_minHeight)
    , m_usingVSync(props.m_vsync)
    , m_useTitlebar(props.m_hasTitlebar)
    , m_open(false)
{
    m_windowID = ++theIDs;
}

} // namespace dogb::UT