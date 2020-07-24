//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_LOGGER_H
#define DOGBONE_UT_LOGGER_H

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "UT_API.h"

#include "UT_IDManager.h"

namespace dogb::UT
{
// Logger for logging useful information to different locations
class UT_API Logger
{
public:
    using addlog_clb_t = std::function<void(const std::string &)>;
    using flush_clb_t = std::function<void()>;
    using callback_id_t = unsigned int;

    void setLog(std::shared_ptr<spdlog::logger> &&logger) { m_log = logger; }

    void addLog(const std::string &log);
    void flush();

    [[maybe_unused]] std::shared_ptr<spdlog::logger> getLog() { return m_log; }

    static Logger &get();

    template <typename... Args>
    static void trace(const char *fmt, Args &&... args)
    {
        get().m_log->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void info(const char *fmt, Args &&... args)
    {
        get().m_log->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void warn(const char *fmt, Args &&... args)
    {
        get().m_log->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void critical(const char *fmt, Args &&... args)
    {
        get().m_log->critical(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void debug(const char *fmt, Args &&... args)
    {
        get().m_log->debug(fmt, std::forward<Args>(args)...);
    }

    callback_id_t registerCallback(addlog_clb_t addlog_clb, flush_clb_t flush_clb)
    {
        callback_id_t id = m_idManager.requestId();

        Listener listener;
        listener.id = id;
        listener.addLogClb = std::move(addlog_clb);
        listener.flushClb = std::move(flush_clb);

        m_listeners.emplace_back(listener);

        return id;
    }

    void removeCallback(callback_id_t id)
    {
        for (auto iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
        {
            if (iter->id == id)
            {
                iter = m_listeners.erase(iter);
                break;
            }
        }
        m_idManager.releaseId(id);
    }

    [[nodiscard]] const std::vector<std::string> &logs() const { return m_logs; }
    void clear() { m_logs.clear(); }

private:

    std::shared_ptr<spdlog::logger> m_log;

    struct Listener
    {
        callback_id_t id{};
        addlog_clb_t addLogClb;
        flush_clb_t flushClb;
    };

    std::vector<std::string> m_logs;
    std::vector<Listener> m_listeners;
    UT::IDManager<callback_id_t> m_idManager;
};

template <typename Mutex>
class LoggerSink : public spdlog::sinks::base_sink<Mutex>
{
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        // Format the message
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        std::string str(fmt::to_string(formatted));

        // Add the log to the subsystem
        UT::Logger &logger = UT::Logger::get();
        logger.addLog(str);
    }

    void flush_() override
    {
        UT::Logger &logger = UT::Logger::get();
        logger.flush();
    }
};

using logger_sink_mt = LoggerSink<std::mutex>;
using logger_sink_st = LoggerSink<spdlog::details::null_mutex>;

#define UT_LOG_TRACE(...) dogb::UT::Logger::trace(__VA_ARGS__);
#define UT_LOG_INFO(...) dogb::UT::Logger::info(__VA_ARGS__);
#define UT_LOG_WARN(...) dogb::UT::Logger::warn(__VA_ARGS__);
#define UT_LOG_ERROR(...) dogb::UT::Logger::critical(__VA_ARGS__);
#define UT_LOG_DEBUG(...) dogb::UT::Logger::debug(__VA_ARGS__);
}

#endif // DOGBONE_UT_LOGGER_H
