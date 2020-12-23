//
// Created by matt on 2020-12-13.
//

#ifndef DOGBONE_GS_HOTKEYMANAGER_H
#define DOGBONE_GS_HOTKEYMANAGER_H

#include "CE_API.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Logger.h>

#include <functional>
#include <stack>
#include <unordered_map>

#include "CE_Input.h"

namespace dogb::CE
{
class DB_CE_API HotKeyConfiguration
{
public:
    using callback_t = std::function<bool(const CE::Input::KeyEvent& )>;

public:
    void setHotKey(const Input::HotKey &hotkey, callback_t clb)
    {
        m_onEvents[hotkey] = std::move(clb);
    }

    bool checkForHandle(const Input::KeyEvent &event) const
    {
        UT_LOG_INFO("Event Size: {}", m_onEvents.size());
        Input::HotKey hotkey(event);
        auto it = m_onEvents.find(hotkey);
        if (it == m_onEvents.end())
            return false;
        if (!it->second)
            return false;
        return it->second(event);
    }

private:
    std::unordered_map<CE::Input::HotKey, callback_t> m_onEvents;
};

class DB_CE_API HotKeyManager
{
public:
    void setGlobalConfiguration(const HotKeyConfiguration &configuration)
    {
        m_globalConfiguration = configuration;
    }

    void setGlobalHotKey(
            const Input::HotKey &hotkey,
            HotKeyConfiguration::callback_t clb)
    {
        m_globalConfiguration.setHotKey(hotkey, std::move(clb));
    }

    bool popConfiguration()
    {
        if (m_configurations.empty())
        {
            UT_ASSERT(m_configurations.empty());
            return false;
        }
        m_configurations.pop();
        return true;
    }
    void pushConfiguration(const HotKeyConfiguration &configuration)
    {
        m_configurations.emplace(configuration);
    }

    void handle(const Input::KeyEvent& ev)
    {
        UT_LOG_INFO("Key Event: k={} a={} m={}", ev.m_key, ev.m_action, ev.m_modifier);
        if (!m_configurations.empty())
        {
            const HotKeyConfiguration &current_config = m_configurations.top();
            if (current_config.checkForHandle(ev))
                return;
        }
        m_globalConfiguration.checkForHandle(ev);
    }
private:
    std::stack<HotKeyConfiguration> m_configurations;

    HotKeyConfiguration m_globalConfiguration;
};
} // namespace dogb::CE

#endif // DOGBONE_GS_HOTKEYMANAGER_H
