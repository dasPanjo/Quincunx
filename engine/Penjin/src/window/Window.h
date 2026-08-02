#pragma once

#include <string>
#include <SDL3/SDL.h>

#include "WindowSettings.h"

namespace Penjin {
    class Window {

    public:
        Window() = default;
        ~Window();



        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool createWindow(const WindowSettings& settings);
        void closeWindow();

        void pollEvents();
        bool shouldClose() const { return shouldClose_; }
        bool swapBuffers() const;


        void setTitle(const std::string & string) const;
        static void setVSync(bool vsync);
    private:
        bool shouldClose_ = false;
        bool sdlInitialized_ = false;
        bool imguiInitialized_ = false;

        SDL_GLContext glContext_ = nullptr;
        SDL_Window* window_ = nullptr;

        SDL_Event event_{};

        void cleanup();

    };
}
