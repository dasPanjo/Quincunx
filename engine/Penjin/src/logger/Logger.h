#pragma once
#include <memory>
#include <vector>
#include <cstdlib>

#include "ILogSink.h"

#define LOG(msg) LOG_INFO(msg)

#define LOG_DEBUG(msg) ::Penjin::Logger::Logger::get().log(::Penjin::Logger::LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_INFO(msg) ::Penjin::Logger::Logger::get().log(::Penjin::Logger::LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_WARN(msg) ::Penjin::Logger::Logger::get().log(::Penjin::Logger::LogLevel::Warning, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_ERROR(msg) ::Penjin::Logger::Logger::get().log(::Penjin::Logger::LogLevel::Error,   __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_FATAL(msg) do { \
    ::Penjin::Logger::Logger::get().log(::Penjin::Logger::LogLevel::Fatal, __FILE__, __LINE__, __FUNCTION__, msg); \
    std::abort(); \
} while (0)

namespace Penjin::Logger {
    class Logger {
    public:
        static Logger& get();
        void addSink(std::shared_ptr<ILogSink> sink);
        void log(LogLevel level, const char* file, int line, const char* function, const std::string& message);
    private:
        std::vector<std::shared_ptr<ILogSink>> sinks_;
    };
}
