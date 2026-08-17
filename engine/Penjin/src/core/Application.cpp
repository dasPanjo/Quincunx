#include "Application.h"

#include <iostream>
#include <format>
#include <stdexcept>

#include <glm/vec4.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "../logger/Logger.h"
#include "../logger/ConsoleSink.h"
#include "../input/Input.h"
#include "../debug/Profiler.h"
#include "../renderer/OpenGLRenderer.h"

Penjin::Application* Penjin::Application::instance_ = nullptr;

Penjin::Application& Penjin::Application::get() {
    if (!instance_) {
        throw std::runtime_error("Penjin::Application::get() called before an Application was constructed");
    }
    return *instance_;
}

Penjin::Application::Application() {
    if (instance_) {
        throw std::runtime_error("Only one Penjin::Application instance may exist at a time");
    }
    instance_ = this;

    Logger::Logger::get().addDefaultSinks();
}

Penjin::Application::~Application() {
    instance_ = nullptr;
}

int Penjin::Application::run(const WindowSettings& settings) {
    errorCode_ = static_cast<int>(ErrorCode::None);

    window_ = std::make_unique<Window>();
    if (!window_->createWindow(settings)) {
        LOG_ERROR("Failed to create window!");
        return static_cast<int>(ErrorCode::WindowCreationFailed);
    }

    renderer_ = std::make_unique<OpenGLRenderer>();
    if (!renderer_->init()) {
        LOG_ERROR("Failed to initialize Renderer!");
        return static_cast<int>(ErrorCode::RendererInitFailed);
    }

    scene_ = std::make_unique<Scene>();
    glm::vec4 clearColor( 0.036f, 0.047f, 0.078f, 1.0f);
    start();
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
        Profiler::draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window_->swapBuffers();
        renderer_->endFrame();
        Profiler::endFrame();
    }
    renderer_->cleanup();

    LOG_DEBUG(std::format("Application is closing with error code {} ({})",
            errorCode_, errorCode_ == 0 ? "Normally" : errorDescription(errorCode_)));
    return errorCode_;
}

void Penjin::Application::quit(ErrorCode code) {
    quit(static_cast<int>(code));
}
void Penjin::Application::quit(int code) {
    errorCode_ = code;
    window_->closeWindow();
}

void Penjin::Application::start() {
}

void Penjin::Application::tick() {
    scene_->tick();
}

void Penjin::Application::draw() {
    scene_->draw(*renderer_);
}
