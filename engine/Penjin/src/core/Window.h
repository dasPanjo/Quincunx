#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Penjin {
    class Window {

    public:
        Window (int width, int height, const std::string& title);
        ~Window();

        void pollEvents();
        bool shouldClose() const { return !isRunning; }
        bool swapBuffers();

    private:
        bool isRunning = true;
        SDL_GLContext glContext;
        SDL_Window* window;

    };
}
