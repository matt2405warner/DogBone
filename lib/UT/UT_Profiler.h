//
// Created by matt on 2020-07-16.
//

#ifndef DOGBONE_UT_PROFILER_H
#define DOGBONE_UT_PROFILER_H

#include "UT_API.h"

#if DB_LINUX
#include <sys/types.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace dogb::UT::Profiler
{
struct UT_API ProfilerResult
{
    std::string m_name;
    std::size_t m_start;
    std::size_t m_end;
    uint32_t m_threadID;
};

class UT_API Profiler
{
public:
    static Profiler &instance();

    void beginSession(
            const std::string &name,
            const std::string &filepath = "profile.json")
    {
        m_stream.open(filepath);
        writeHeader();
        m_session = name;
    }

    void endSession()
    {
        writeFooter();
        m_stream.close();
        m_session.clear();
        m_count = 0;
    }

    void writeHeader()
    {
        m_stream << R"({"otherData": {}, "traceEvents":[)";
        m_stream.flush();
    }

    void writeFooter()
    {
        m_stream << "]}";
        m_stream.flush();
    }

    void write(const ProfilerResult &result)
    {
        if (m_count > 0)
            m_stream << ',';

        m_count++;

        std::string name = result.m_name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_stream << '{';
        m_stream << R"("cat":"function",)";
        m_stream << R"("dur":)" << (result.m_end - result.m_start) << ',';
        m_stream << R"("name":")" << name << "\",";
        m_stream << R"("ph":"X",)";
        m_stream << R"("pid":)" << getpid() << ',';
        m_stream << R"("tid":)" << result.m_threadID << ',';
        m_stream << R"("ts":)" << result.m_start;
        m_stream << '}';

        m_stream.flush();
    }

private:
    Profiler() : m_count(0), m_session() {}

    uint32_t m_count;
    std::ofstream m_stream;
    std::string m_session;
};

class UT_API Timer
{
public:
    explicit Timer(const char *name) : m_isStopped(false), m_name(name)
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_isStopped)
            stop();
    }

    void stop()
    {
        if (m_isStopped)
            return;

        auto end_time = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(
                             m_startTime)
                             .time_since_epoch()
                             .count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(
                           end_time)
                           .time_since_epoch()
                           .count();

        m_isStopped = true;

        uint32_t thread_id = static_cast<uint32_t>(
                std::hash<std::thread::id>{}(std::this_thread::get_id()));
        Profiler::instance().write(
                {m_name, static_cast<std::size_t>(start),
                 static_cast<std::size_t>(end), thread_id});
    }

private:
    bool m_isStopped;
    const char *m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};
} // namespace dogb::UT::Profiler

#define UT_ENABLE_PROFILER

#ifdef UT_ENABLE_PROFILER
#  define __UT_COMBINE(x, y) x ## y

#  define UT_PROFILE_BEGIN_SESSION(name, filepath) dogb::UT::Profiler::Profiler::instance().beginSession(name, filepath);
#  define UT_PROFILE_SCOPE(name) dogb::UT::Profiler::Timer __UT_COMBINE(timer, __LINE__)(name);
#if DB_LINUX
#    define UT_PROFILE_FUNCTION() UT_PROFILE_SCOPE(__FUNCTION__);
#else
#    define UT_PROFILE_FUNCTION() UT_PROFILE_SCOPE(__FUNCSIG__);
#endif
#  define UT_PROFILE_END_SESSION() dogb::UT::Profiler::Profiler::instance().endSession();
#else
#  define UT_PROFILE_BEGIN_SESSION(name)
#  define UT_PROFILE_END_SESSION()
#  define UT_PROFILE_FUNCTION()
#  define UT_PROFILE_SCOPE(name)
#endif

#endif // DOGBONE_UT_PROFILER_H
