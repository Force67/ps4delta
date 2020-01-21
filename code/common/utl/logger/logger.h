#pragma once

#include <chrono>
#include <cstdint>

// hard dependency
#include <fmt/format.h>

// log impl is heavily influenced & based on the yuzu logger

namespace utl {
enum class logLevel : uint8_t {
    Trace,    //< very detailed and extensive debugging information
    Debug,    //< less detailed debug information
    Info,     //< status information
    Warning,  //< minor or potential problems
    Error,    //< major problems that block further execution of a task
    Critical, //< major problems that block continued execution of the entire
              // application
    Count     //< total number of log levels
};

struct logEntry {
    std::chrono::microseconds timestamp;
    logLevel log_level;
    unsigned int line_num;
    std::string function;
    std::string message;
    bool final_entry = false;

    logEntry() = default;
    logEntry(logEntry&& o) = default;

    logEntry& operator=(logEntry&& o) = default;
    logEntry& operator=(const logEntry& o) = default;
};

class logBase {
public:
    virtual ~logBase() = default;

    virtual const char* getName() {
        return "Unknown";
    }

    virtual void write(const logEntry&) = 0;
};

std::string formatLogEntry(const logEntry& entry);
logBase* addLogSink(std::unique_ptr<logBase> sink);
logBase* getLogSink(std::string_view name);
void formatLogMsg(logLevel lvl, uint32_t line, const char* func, const char* fmt,
                  const fmt::format_args& args);

void createLogger(bool withConsole = false);

template <typename... Args>
inline void fmtLogMsg(logLevel lvl, uint32_t line, const char* func, const char* fmt,
                      const Args&... args) {
    formatLogMsg(lvl, line, func, fmt, fmt::make_format_args(args...));
}

template <typename... Args>
inline void fmtLogMsg(logLevel lvl, uint32_t line, const char* func, const std::string& text) {
    formatLogMsg(lvl, line, func, text.c_str(), {});
}
}

#ifdef _DEBUG
#define LOG_TRACE(...) ::utl::fmtLogMsg(::utl::logLevel::Trace, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_TRACE(fmt, ...) (void(0))
#endif

#define LOG_DEBUG(...) ::utl::fmtLogMsg(::utl::logLevel::Debug, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(...) ::utl::fmtLogMsg(::utl::logLevel::Info, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARNING(...) ::utl::fmtLogMsg(::utl::logLevel::Warning, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(...) ::utl::fmtLogMsg(::utl::logLevel::Error, __LINE__, __func__, __VA_ARGS__)
#define LOG_CRITICAL(...)                                                                          \
    ::utl::fmtLogMsg(::utl::logLevel::Critical, __LINE__, __func__, __VA_ARGS__)
#define LOG_ASSERT(expression)                                                                     \
    do {                                                                                           \
        if (!(expression)) {                                                                       \
            ::utl::fmtLogMsg(::utl::logLevel::Error, __LINE__, __func__,                           \
                             "assertion failed at " #expression);                                  \
            __debugbreak();                                                                        \
        }                                                                                          \
    \
} while (0)

#define LOG_UNIMPLEMENTED                                                                          \
    ::utl::fmtLogMsg(::utl::logLevel::Error, __LINE__, __func__, "Unimplemented function")
