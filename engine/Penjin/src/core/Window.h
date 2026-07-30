#pragma once

#include <iostream>

namespace Penjin {
    class Window {

    public:
        Window (int width, int height, const std::string& title);
        ~Window();

        void pollEvents();
        bool shouldClose() const;
        bool swapBuffers();

    };
}
