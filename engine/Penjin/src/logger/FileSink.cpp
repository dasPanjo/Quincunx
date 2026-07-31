#include "FileSink.h"

Penjin::Logger::FileSink::FileSink(const std::string &path, LogLevel minLevel) :
    ILogSink(minLevel),
    file_(path, std::ios::app) {

}

void Penjin::Logger::FileSink::write(LogLevel level, std::string_view message) {
    if (level < getMinLevel())
        return;

    if (file_.is_open()) {
        file_ << message << '\n';
    }
}
