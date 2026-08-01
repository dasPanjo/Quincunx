#include "WindowSettings.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <format>
#include <string_view>

#include "../logger/Logger.h"

namespace {
    bool parseInt(std::string_view text, int& outValue) {
        const auto result = std::from_chars(text.data(), text.data() + text.size(), outValue);
        return result.ec == std::errc{} && result.ptr == text.data() + text.size();
    }

    bool equalsIgnoreCase(std::string_view a, std::string_view b) {
        if (a.size() != b.size()) {
            return false;
        }

        return std::equal(a.begin(), a.end(), b.begin(),
            [](unsigned char c1, unsigned char c2) {
                return std::tolower(c1) == std::tolower(c2);
            });
    }
}

std::string Penjin::WindowSettings::toString(WindowMode windowMode) {
    switch (windowMode) {
        case WindowMode::Windowed: return "Windowed";
        case WindowMode::Borderless: return "Borderless";
        case WindowMode::Fullscreen: return "Fullscreen";
        default: return "Unknown";
    }
}

std::optional<Penjin::WindowSettings::WindowMode> Penjin::WindowSettings::fromString(std::string_view text) {
    if (equalsIgnoreCase(text, "windowed"))   return WindowMode::Windowed;
    if (equalsIgnoreCase(text, "borderless")) return WindowMode::Borderless;
    if (equalsIgnoreCase(text, "fullscreen")) return WindowMode::Fullscreen;
    return std::nullopt;
}

Penjin::WindowSettings Penjin::WindowSettings::fromArgs(int argc, char** argv) {
    WindowSettings settings;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg = argv[i];

        if (arg == "--vsync") {
            settings.vsync = true;
        }
        else if (arg == "--no-vsync") {
            settings.vsync = false;
        }
        else if (arg == "--windowed") {
            settings.displayMode = WindowMode::Windowed;
        }
        else if (arg == "--borderless") {
            settings.displayMode = WindowMode::Borderless;
        }
        else if (arg == "--fullscreen") {
            settings.displayMode = WindowMode::Fullscreen;
        }
        else if (arg == "--window-mode") {
            if (i + 1 < argc) {
                if (const auto mode = WindowSettings::fromString(argv[++i])) {
                    settings.displayMode = *mode;
                } else {
                    LOG_WARN(std::format("Unknown window-mode '{}', using default ({})", argv[i], toString(settings.displayMode)));
                }
            } else {
                LOG_WARN("Missing value for --window-mode, using default");
            }
        }
        else if (arg == "--width") {
            if (i + 1 >= argc || !parseInt(argv[++i], settings.width)) {
                LOG_WARN(std::format("Invalid or missing value for --width, using default ({})", settings.width));
            }
        }
        else if (arg == "--height") {
            if (i + 1 >= argc || !parseInt(argv[++i], settings.height)) {
                LOG_WARN(std::format("Invalid or missing value for --height, using default ({})", settings.height));
            }
        }
    }

    return settings;
}
