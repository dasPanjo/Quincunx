#pragma once

#include "Window.h"

namespace Penjin {
    class Application {
    public:
        Application();
        virtual ~Application();
        int run();

    private:
        Window* window;

    };
}
