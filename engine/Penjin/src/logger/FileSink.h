#pragma once

#include <fstream>
#include <string>

#include "ILogSink.h"

namespace Penjin::Logger {
    class FileSink : public ILogSink {
        public:
        explicit FileSink(const std::string& path, LogLevel minLevel = LogLevel::Warning);
        void write(LogLevel level, std::string_view message) override;

    private:
        std::ofstream file_;
    };
}
