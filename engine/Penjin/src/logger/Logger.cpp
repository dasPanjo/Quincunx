#include "Logger.h"

#include <cstring>
#include <sstream>

Penjin::Logger::Logger & Penjin::Logger::Logger::get() {
    static Logger instance;
    return instance;
}

void Penjin::Logger::Logger::addSink(std::shared_ptr<ILogSink> sink) {
    sinks_.push_back(std::move(sink));
}

void Penjin::Logger::Logger::log(
    LogLevel level,
    const char* file,
    int line,
    const char* function,
    const std::string& message) {

    const char* filename = std::strrchr(file, '\\');
    if (!filename) {
        filename = std::strrchr(file, '/');
    }
    filename = filename ? filename + 1 : file;

    std::ostringstream formattedMessage;
    formattedMessage << "[" << toString(level) << "] " << filename << ":" << line << " (" << function << ") - " << message;

    for(const auto& sink : sinks_) {
        sink->write(level, formattedMessage.str());
    }
}