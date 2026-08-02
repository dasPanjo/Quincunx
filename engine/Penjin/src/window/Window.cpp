#include "Window.h"

#include <glad/glad.h>
#include <iostream>
#include <format>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "../logger/Logger.h"
#include "WindowSettings.h"
#include "../input/Input.h"


namespace {
    using WindowMode = ::Penjin::WindowSettings::WindowMode;
}

Penjin::Window::~Window() {
    cleanup();
}

bool Penjin::Window::createWindow(const WindowSettings& settings) {

    // Init SDL with the video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR(std::format("SDL could not initialize! SDL Error: {}", SDL_GetError()));
        return false;
    }

    // Set OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL v3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL v3.3
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true); // Double buffer

    // Create SDL Window
    unsigned int windowFlags = SDL_WINDOW_OPENGL;
    switch (settings.displayMode) {
        case WindowMode::Windowed: break;
        case WindowMode::Borderless: break;
        case WindowMode::Fullscreen: windowFlags |= SDL_WINDOW_FULLSCREEN; break;
    }
    window_ = SDL_CreateWindow(
        settings.title.c_str(),
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

    // ImGui initialization
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    if (!ImGui_ImplSDL3_InitForOpenGL(window_, glContext_)) {
        LOG_ERROR("Failed to initialize ImGui SDL3 backend");
        ImGui::DestroyContext();
        cleanup();
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        LOG_ERROR("Failed to initialize ImGui OpenGL3 backend");
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        cleanup();
        return false;
    }
    imguiInitialized_ = true;

    // Debug output
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    LOG_DEBUG(std::format("OpenGL version: {}", version ? version : "Unknown"));
    LOG_DEBUG(std::format("GPU: {}", renderer ? renderer : "Unknown"));

    return true;
}

void Penjin::Window::pollEvents() {
    while (SDL_PollEvent(&event_)) {
        if (event_.type == SDL_EVENT_QUIT) {
            shouldClose_ = true;
        }

        ImGui_ImplSDL3_ProcessEvent(&event_);

        Input::get().processEvent(event_);
        if (Input::get().isKeyPressed(Escape)) {
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
    if (!SDL_GL_SetSwapInterval(vsync ? 1 : 0)) {
        LOG_WARN(std::format("SDL_GL_SetSwapInterval failed: {}", SDL_GetError()));
    }
}

void Penjin::Window::cleanup() {
    if (imguiInitialized_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        imguiInitialized_ = false;
    }

    if (glContext_) {
        SDL_GL_DestroyContext(glContext_);
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
