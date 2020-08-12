//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_WINDOW_H
#define DOGBONE_UT_WINDOW_H

#include "UT_API.h"

#include "UT_Logger.h"
#include "UT_Timestep.h"

#include <SYS/SYS_Types.h>

namespace dogb::UT
{
class SubSystem;

struct DB_UT_API WindowProperties
{
    WindowProperties() = default;
    WindowProperties(
            const String &title,
            U32 width,
            U32 height,
            U32 min_width,
            U32 min_height,
            bool has_title_bar,
            bool vsyc);

    String m_title;
    U32 m_width{};
    U32 m_height{};
    U32 m_minWidth{};
    U32 m_minHeight{};
    bool m_hasTitlebar{};
    bool m_vsync{};
};

class DB_UT_API Window
{
public:
    using IDType = U32;

    explicit Window(const WindowProperties &props);
    virtual ~Window() = default;

    [[nodiscard]] U32 width() const { return m_width; }
    [[nodiscard]] U32 height() const { return m_height; }
    [[nodiscard]] U32 minWidth() const { return m_minWidth; }
    [[nodiscard]] U32 minHeight() const { return m_minHeight; }

    [[nodiscard]] const String &title() const { return m_title; }
    [[nodiscard]] bool isUsingVSync() const { return m_usingVSync; }
    [[nodiscard]] bool isOpen() const { return m_open; }
    [[nodiscard]] IDType id() const { return m_windowID; }

    virtual void setVSync(bool enabled) = 0;
    virtual void close() { m_open = false; }

    virtual void preUpdate() = 0;
    virtual void update(UT::Timestep ts) = 0;
    virtual void postUpdate() = 0;
    virtual void use() = 0;
    virtual void shutdown() = 0;

    virtual void *nativeWindow() = 0;

    // Initialize any sub system contexts the window may require
    virtual void initialize() = 0;

protected:
    String m_title;
    U32 m_width;
    U32 m_height;
    U32 m_minWidth;
    U32 m_minHeight;
    bool m_usingVSync;
    bool m_useTitlebar;
    bool m_open;

private:
    IDType m_windowID;
};
} // namespace dogb::UT

#endif // DOGBONE_UT_WINDOW_H
