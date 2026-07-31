#include "Window.h"
#include <glad/glad.h>

#include <iostream>

Penjin::Window::~Window() {
    cleanup();
}

bool Penjin::Window::createWindow(const std::string& title, int width, int height) {
    // Init SDL with the video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL v3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL v3.3
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE); // Double buffer

    // Create SDL Window
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window_) {
        std::cerr << "Unable to create SDL Window! SDL Error: " << SDL_GetError() << std::endl;
        cleanup();
        return false;
    }
    sdlInitialized_ = true;


    // OpenGL Context
    glContext_ = SDL_GL_CreateContext(window_);
    if (!glContext_) {
        std::cerr << "Unable to create GL context! SDL Error: " << SDL_GetError() << std::endl;
        cleanup();
        return false;
    }

    // OpenGL Loader
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        cleanup();
        return false;
    }

    // Debug output
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
    return true;
}

void Penjin::Window::pollEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            shouldClose_ = true;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            shouldClose_ = true;
        }
    }
}

bool Penjin::Window::swapBuffers() {
    SDL_GL_SwapWindow(window_);
    return true;
}

void Penjin::Window::cleanup() {
    if (glContext_) {
        SDL_GL_DeleteContext(glContext_);
        glContext_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    if (sdlInitialized_) {
        SDL_Quit();
        sdlInitialized_ = false;
    }
}
