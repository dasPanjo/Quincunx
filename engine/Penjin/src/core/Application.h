#pragma once

#include <memory>

#include "ErrorCodes.h"
#include "../time/Time.h"
#include "../window/Window.h"
#include "../renderer/IRenderer.h"
#include "../scene/Scene.h"

namespace Penjin {
    class Application {
    public:
        static Application& get();

        Application();
        virtual ~Application();

        virtual int run(const WindowSettings& settings);

        [[nodiscard]] Window& window() const { return *window_; }
        [[nodiscard]] Scene& scene() const { return *scene_; }

        void quit(ErrorCode errorCode);
        void quit(int code = 0);
    protected:
        virtual void start();
        virtual void tick();
        virtual void draw();
    private:
        static Application* instance_;

        std::unique_ptr<Window> window_;
        std::unique_ptr<IRenderer> renderer_;
        std::unique_ptr<Scene> scene_;

        int errorCode_;
    };
}
