#include "Window.h"

#include <glad/glad.h>
#include <format>
#include <iostream>

#include "../logger/Logger.h"

Penjin::Window::~Window() {
    cleanup();
}

bool Penjin::Window::createWindow(const std::string& title, int width, int height) {
    // Init SDL with the video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_ERROR(std::format("SDL could not initialize! SDL Error: {}", SDL_GetError()));
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
        LOG_ERROR(std::format("Unable to create SDL Window! SDL Error: {}", SDL_GetError()));
        cleanup();
        return false;
    }
    sdlInitialized_ = true;


    // OpenGL Context
    glContext_ = SDL_GL_CreateContext(window_);
    if (!glContext_) {
        LOG_ERROR(std::format("Unable to create GL context! SDL Error: {}", SDL_GetError()));
        cleanup();
        return false;
    }

    // OpenGL Loader
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        LOG_ERROR("Failed to initialize GLAD");
        cleanup();
        return false;
    }

    // Debug output
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    LOG_DEBUG(std::format("OpenGL version: {}", version ? version : "Unknown"));
    LOG_DEBUG(std::format("GPU: {}", renderer ? renderer : "Unknown"));

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
