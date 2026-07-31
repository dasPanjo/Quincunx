#pragma once

#include "Window.h"

#include <memory>

namespace Penjin {
    class Application {
    public:
        Application();
        virtual ~Application();
        int run();

    private:
        std::unique_ptr<Window> window_;
    };
}
