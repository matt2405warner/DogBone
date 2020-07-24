//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_WINDOW_H
#define DOGBONE_UT_WINDOW_H

#include "UT_API.h"

#include "UT_Engine.h"
#include "UT_Logger.h"
#include "UT_SubSystemContext.h"
#include "UT_Timestep.h"

#include <SYS/SYS_Types.h>

namespace dogb::UT
{
class SubSystem;

struct UT_API WindowProperties
{
    WindowProperties() = default;
    WindowProperties(
            const String &title,
            I32 width,
            I32 height,
            I32 min_width,
            I32 min_height,
            bool has_title_bar,
            bool vsyc);

    String m_title;
    I32 m_width{};
    I32 m_height{};
    I32 m_minWidth{};
    I32 m_minHeight{};
    bool m_hasTitlebar{};
    bool m_vsync{};
};

class UT_API Window
{
public:
    using IDType = U32;

    explicit Window(const WindowProperties &props);
    virtual ~Window() = default;

    [[nodiscard]] I32 width() const { return m_width; }
    [[nodiscard]] I32 height() const { return m_height; }
    [[nodiscard]] I32 minWidth() const { return m_minWidth; }
    [[nodiscard]] I32 minHeight() const { return m_minHeight; }

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
    I32 m_width;
    I32 m_height;
    I32 m_minWidth;
    I32 m_minHeight;
    bool m_usingVSync;
    bool m_useTitlebar;
    bool m_open;

    template <typename SSTy, typename CtxTy, typename ... Args>
    std::shared_ptr<CtxTy> addContext(Args&& ...args)
    {
        static_assert(
                std::is_base_of_v<UT::SubSystemContext, CtxTy>,
                "Context to create must derive from rne::UT::SubSystemContext");
        static_assert(
                std::is_base_of_v<UT::SubSystem, SSTy>,
                "SubSystem Type must derive from rne::UT::SubSystem");
        static_assert(
                std::is_member_function_pointer_v<decltype(&SSTy::attach)>,
                "SubSystem Type must have 'attach()' member function");

        SSTy *system = UT::Engine::get().getOrCreateSubSystem<SSTy>();
        if (system == nullptr)
        {
            UT_LOG_ERROR("Failed to create subsystem");
            return nullptr;
        }
        UT_LOG_DEBUG("Creating context");
        auto ctx = std::make_shared<CtxTy>(std::forward<Args>(args)...);
        if (!ctx)
        {
            UT_LOG_ERROR("Failed to create subsystem context");
            return nullptr;
        }
        system->attach(ctx);
        m_contexts.emplace_back(ctx);

        return ctx;
    }

    std::vector<std::weak_ptr<SubSystemContext>> m_contexts;
private:
    IDType m_windowID;
};
} // namespace dogb::UT

#endif // DOGBONE_UT_WINDOW_H
