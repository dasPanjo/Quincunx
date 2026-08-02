#pragma once

#include <memory>

#include "../time/Time.h"
#include "../window/Window.h"

namespace Penjin {
    class Application {
    public:
        Application();
        virtual ~Application();
        virtual int run(const WindowSettings& settings);

    protected:
        std::unique_ptr<Window> window_;

        virtual void tick() {}
        virtual void draw() {}
    };
}
