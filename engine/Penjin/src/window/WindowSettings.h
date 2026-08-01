#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Penjin {
    struct WindowSettings {
        enum class WindowMode {
            Windowed,
            Borderless,
            Fullscreen,
        };

        static std::string toString(WindowMode windowMode);

        static std::optional<WindowMode> fromString(std::string_view text);

        std::string title = "Quincunx";
        int width = 1280;
        int height = 720;
        bool vsync = true;
        WindowMode displayMode = WindowMode::Borderless;

        static WindowSettings fromArgs(int argc, char** argv);
    };
}
