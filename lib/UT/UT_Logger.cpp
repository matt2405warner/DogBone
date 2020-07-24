//
// Created by matt on 2020-07-11.
//

#include "UT_Logger.h"

namespace dogb::UT
{
Logger &
Logger::get()
{
    static Logger _instance;
    return _instance;
}

void
Logger::addLog(const std::string &log)
{
    static constexpr int s_max_log_size = 1000;
    if (m_logs.size() >= s_max_log_size)
        m_logs.erase(m_logs.begin());
    m_logs.emplace_back(log);
    for (auto &listener : m_listeners)
    {
        if (!listener.addLogClb)
            continue;
        listener.addLogClb(log);
    }
}

void
Logger::flush()
{
    for (auto &listener : m_listeners)
    {
        if (!listener.flushClb)
            continue;

        listener.flushClb();
    }
}

} // namespace dogb::UT