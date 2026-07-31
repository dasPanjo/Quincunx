#pragma once
#include <iostream>

#include "ILogSink.h"

namespace Penjin::Logger {
    class ConsoleSink : public ILogSink {
    public:
        explicit ConsoleSink(LogLevel minLevel = LogLevel::Debug) : ILogSink(minLevel) {}
        void write(LogLevel level, std::string_view message) override {
            if (level < getMinLevel())
                return;

            if (level >= LogLevel::Error) {
                std::cerr << message << std::endl;
            }else {
                std::cout << message << std::endl;
            }
        }
    };
}
