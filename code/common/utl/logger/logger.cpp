
#include <mutex>
#include <thread>
#include <vector>

#include "logger.h"
#include "threadsafe_queue.h"

namespace utl {
template <typename InIt>
bool ComparePartialString(InIt begin, InIt end, const char* other) {
    for (; begin != end && *other != '\0'; ++begin, ++other) {
        if (*begin != *other) {
            return false;
        }
    }
    // Only return true if both strings finished at the same point
    return (begin == end) == (*other == '\0');
}

class LogRegistry {
    std::mutex writing_lock;
    std::thread backend_thread;
    std::vector<std::unique_ptr<logBase>> sinks;
    Common::MPSCQueue<logEntry> pending;
    std::chrono::steady_clock::time_point time_origin;

public:
    LogRegistry(LogRegistry const&) = delete;
    const LogRegistry& operator=(LogRegistry const&) = delete;

    static LogRegistry& Instance() {
        static LogRegistry backend;
        return backend;
    }

    LogRegistry() {

        time_origin = std::chrono::steady_clock::now();

        backend_thread = std::thread([&] {
            logEntry entry;
            auto write_logs = [&](logEntry& e) {
                std::lock_guard lock{writing_lock};
                for (const auto& sink : sinks) {
                    sink->write(e);
                }
            };

            // work the queue
            while (true) {
                entry = pending.PopWait();

                // do we need to drain?
                if (entry.final_entry)
                    break;

                write_logs(entry);
            }

            // drain the queue
            // only writes up to MAX_LOGS
            constexpr int MAX_LOGS_TO_WRITE = 100;
            int logs_written = 0;
            while (logs_written++ < MAX_LOGS_TO_WRITE && pending.Pop(entry)) {
                write_logs(entry);
            }
        });
    }

    ~LogRegistry() {
        logEntry entry;
        entry.final_entry = true;
        pending.Push(entry);
        backend_thread.join();
    }

    inline void AddEntry(logLevel lvl, uint32_t line, const char* func, std::string msg) {

        using std::chrono::duration_cast;
        using std::chrono::steady_clock;

        logEntry entry{};
        entry.timestamp =
            duration_cast<std::chrono::microseconds>(steady_clock::now() - time_origin);
        entry.log_level = lvl;
        entry.line_num = line;
        entry.function = func;
        entry.message = std::move(msg);

        pending.Push(entry);
    }

    inline logBase* AddSink(std::unique_ptr<logBase> sink) {
        std::lock_guard lock{writing_lock};
        auto* poop = sink.get();

        sinks.push_back(std::move(sink));
        return poop;
    }

    inline void RemoveSink(std::string_view name) {
        std::lock_guard lock{writing_lock};
        const auto it = std::remove_if(sinks.begin(), sinks.end(),
                                       [&name](const auto& i) { return name == i->getName(); });
        sinks.erase(it, sinks.end());
    }

    inline logBase* GetSink(std::string_view name) {
        const auto it = std::find_if(sinks.begin(), sinks.end(),
                                     [&name](const auto& i) { return name == i->getName(); });
        if (it == sinks.end())
            return nullptr;
        return it->get();
    }
};

const char* GetLevelName(logLevel log_level) {
#define LVL(x)                                                                                     \
    case logLevel::x:                                                                              \
        return #x
    switch (log_level) {
        LVL(Trace);
        LVL(Debug);
        LVL(Info);
        LVL(Warning);
        LVL(Error);
        LVL(Critical);
        // case LogLevel::Count:
        // UNREACHABLE();
    }
#undef LVL
    return nullptr;
}

std::string formatLogEntry(const logEntry& entry) {
    uint32_t time_seconds = static_cast<unsigned int>(entry.timestamp.count() / 1000000);
    uint32_t time_fractional = static_cast<unsigned int>(entry.timestamp.count() % 1000000);

    const char* level_name = GetLevelName(entry.log_level);

    return fmt::format("[{:4d}.{:06d}] <{}> {}:{}: {}", time_seconds, time_fractional, level_name,
                       entry.function, entry.line_num, entry.message);
}

logBase* addLogSink(std::unique_ptr<logBase> sink) {
    return LogRegistry::Instance().AddSink(std::move(sink));
}

void formatLogMsg(logLevel lvl, uint32_t line, const char* func, const char* fmt,
                  const fmt::format_args& args) {
    auto& reg = LogRegistry::Instance();
    reg.AddEntry(lvl, line, func, fmt::vformat(fmt, args));
}

logBase* getLogSink(std::string_view name) {
    auto& reg = LogRegistry::Instance();
    return reg.GetSink(name);
}
}