//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_APPLICATION_H
#define DOGBONE_UT_APPLICATION_H

#include "UT_API.h"

#include "UT_Window.h"
#include "UT_Logger.h"

#include <memory>
#include <utility>

namespace dogb::UT
{
class DB_UT_API Application
{
public:
    explicit Application(std::string  name) :
        m_name(std::move(name)), m_isRunning(false)
    {}
    virtual ~Application() = default;

    [[nodiscard]] bool isRunning() const { return m_isRunning; }
    void quitApp() { m_isRunning = false; }

    template <typename T>
    T *createWindow(const WindowProperties &properties)
    {
        static_assert(
                std::is_base_of_v<Window, T>,
                "Create requires type to derive from Window.");

        m_mainWindow = std::make_unique<T>(properties);
        m_mainWindow->initialize();
        return reinterpret_cast<T *>(m_mainWindow.get());
    }

    [[nodiscard]] Window* mainWindow() const { return m_mainWindow.get(); }
protected:
    virtual std::unique_ptr<Application> create() { return nullptr; };

    virtual int initialize(int argc, char *argv[]) = 0;
    virtual int exec() { m_isRunning = true; return 1; }
    virtual int cleanup();

    std::unique_ptr<Window> m_mainWindow;

    std::string m_name;
    bool m_isRunning;

    friend class AppInterface;
};

class DB_UT_API AppInterface
{
public:
    virtual ~AppInterface() = default;

    int run(int argc, char *argv[]);

    static Application *mainApp() { return s_instance.get(); }

protected:
    virtual std::unique_ptr<Application> create() { return nullptr; }

private:
    static std::unique_ptr<Application> s_instance;
};
} // namespace dogb::UT

#endif // DOGBONE_UT_APPLICATION_H
