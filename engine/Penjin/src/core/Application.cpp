#include "Application.h"

#include <iostream>
#include <format>

#include <glm/vec4.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "../logger/Logger.h"
#include "../logger/ConsoleSink.h"
#include "../input/Input.h"
#include "../renderer/OpenGLRenderer.h"


Penjin::Application::Application() {
    Logger::Logger::get().addDefaultSinks();
}

Penjin::Application::~Application() = default;

int Penjin::Application::run(const WindowSettings& settings) {
    window_ = std::make_unique<Window>();
    if (!window_->createWindow(settings)) {
        LOG_ERROR("Failed to create window!");
        return 1;
    }

    renderer_ = std::make_unique<OpenGLRenderer>();
    if (!renderer_->init()) {
        LOG_ERROR("Failed to initialize Renderer!");
        return 1;
    }

    glm::vec4 clearColor( 0.036f, 0.047f, 0.078f, 1.0f);
    while (!window_->shouldClose()) {
        Time::get().tick();
        Input::get().beginFrame();
        window_->pollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        tick();


        renderer_->beginFrame(clearColor);
        draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window_->swapBuffers();
        renderer_->endFrame();

    }

    renderer_->cleanup();
    return 0;
}

void Penjin::Application::quit(int code) {
    LOG_DEBUG(std::format("Application is closing with code {}{}", code, code == 0 ? "(Normally)" : ""));
    window_->closeWindow();
}

void Penjin::Application::tick() {
    window_->setTitle(std::format("Quincunx - {:0.3f}ms ({:3.0f}fps)", Time::get().deltaTimeMs(), 1.0f / Time::get().deltaTime()));
}

void Penjin::Application::draw() {
}
