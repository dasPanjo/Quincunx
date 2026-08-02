#pragma once
#include <string_view>
#include "LogLevel.h"

namespace Penjin::Logger {
    class ILogSink {
    public:
        explicit ILogSink(LogLevel minLevel = LogLevel::Info) : minLevel_(minLevel) {}

        virtual ~ILogSink() = default;
        virtual void write(
            LogLevel level,
            std::string_view message) = 0;

        [[nodiscard]] LogLevel getMinLevel() const { return minLevel_; }
    private:
        LogLevel minLevel_;
    };
}