#pragma once

namespace Penjin::Logger {
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
    };

    constexpr std::string_view toString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "Debug";
            case LogLevel::Info: return "Info";
            case LogLevel::Warning: return "Warning";
            case LogLevel::Error: return "Error";
            case LogLevel::Fatal: return "Fatal";
        }
        return "Unknown";
    }
}