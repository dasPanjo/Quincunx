#pragma once

#include <memory>

#include "../time/Time.h"
#include "../window/Window.h"
#include "../renderer/IRenderer.h"

namespace Penjin {
    class Application {
    public:
        Application();
        virtual ~Application();
        virtual int run(const WindowSettings& settings);

        void quit(int code = 0);
    protected:
        std::unique_ptr<Window> window_;
        std::unique_ptr<IRenderer> renderer_;

        virtual void tick();
        virtual void draw();
    };
}
