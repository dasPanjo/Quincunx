#include "Window.h"

#include <glad/glad.h>
#include <format>
#include <iostream>

#include "../logger/Logger.h"
#include "WindowSettings.h"

namespace {
    using WindowMode = ::Penjin::WindowSettings::WindowMode;
}

Penjin::Window::~Window() {
    cleanup();
}

bool Penjin::Window::createWindow(const WindowSettings& settings) {

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
    unsigned int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    switch (settings.displayMode) {
        case WindowMode::Windowed: break;
        case WindowMode::Borderless: windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
        case WindowMode::Fullscreen: windowFlags |= SDL_WINDOW_FULLSCREEN; break;
    }
    window_ = SDL_CreateWindow(
        settings.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        settings.width, settings.height,
        windowFlags);
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

    setVSync(settings.vsync);

    // Debug output
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    LOG_DEBUG(std::format("OpenGL version: {}", version ? version : "Unknown"));
    LOG_DEBUG(std::format("GPU: {}", renderer ? renderer : "Unknown"));

    return true;
}

void Penjin::Window::pollEvents() {
    while (SDL_PollEvent(&event_)) {
        if (event_.type == SDL_QUIT) {
            shouldClose_ = true;
        }
        if (event_.type == SDL_KEYDOWN && event_.key.keysym.sym == SDLK_ESCAPE) {
            shouldClose_ = true;
        }
    }
}

bool Penjin::Window::swapBuffers() const {
    SDL_GL_SwapWindow(window_);
    return true;
}
void Penjin::Window::setTitle(const std::string &string) const {
    SDL_SetWindowTitle(window_, string.c_str());
}
void Penjin::Window::setVSync(bool vsync) {
    if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) != 0) {
        LOG_WARN(std::format("SDL_GL_SetSwapInterval failed: {}", SDL_GetError()));
    }
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
