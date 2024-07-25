#pragma once

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/color.h>

#if defined(AL_SERVER) || defined(AL_DEBUG)
#define Log(...) ::Aluminium::Logger::LogMsg(__VA_ARGS__)
#define LogWarn(...) ::Aluminium::Logger::LogWarnMsg(__VA_ARGS__)
#define LogError(...) ::Aluminium::Logger::LogErrorMsg(__VA_ARGS__)
#else
#define Log(...)
#define LogWarn(...)
#define LogError(...)
#endif

namespace Aluminium {

    class Logger {

    public:
        Logger();

        static inline void LogMsg(const char* msg) { OutputMsg(msg); }
        static inline void LogWarnMsg(const char* msg) { OutputMsg(msg); }
        static inline void LogErrorMsg(const char* msg) { OutputMsg(msg); }

        template<typename... Args> static inline void LogMsg(const char* fmt, Args&&... args) { OutputMsg(fmt::format(fmt, args...)); }
        template<typename... Args> static inline void LogWarnMsg(const char* fmt, Args&&... args) { OutputMsg(fmt::format(fmt, args...)); }
        template<typename... Args> static inline void LogErrorMsg(const char* fmt, Args&&... args) { OutputMsg(fmt::format(fmt, args...)); }
        
    private:
        static void OutputMsg(const std::string& msg);

    };

}
