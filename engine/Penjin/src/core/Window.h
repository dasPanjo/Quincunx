#pragma once

#include <string>
#include <SDL2/SDL.h>

namespace Penjin {
    class Window {

    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool createWindow(const std::string& title, int width, int height);

        void pollEvents();
        bool shouldClose() const { return shouldClose_; }
        bool swapBuffers();

    private:
        bool shouldClose_ = false;
        bool sdlInitialized_ = false;

        SDL_GLContext glContext_ = nullptr;
        SDL_Window* window_ = nullptr;

        SDL_Event event{};

        void cleanup();

    };
}
