#pragma once

#include <memory>

#include "Window.h"


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
