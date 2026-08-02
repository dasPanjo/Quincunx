#include "Application.h"

#include <iostream>
#include <format>

#include <glad/glad.h>
#include <glm/vec4.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "../logger/Logger.h"
#include "../logger/ConsoleSink.h"
#include "../input/Input.h"


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

    // Trivial GLM usage to confirm it's wired up correctly.
    glm::vec4 clearColor( 0.036f, 0.047f, 0.078f, 1.0f);

    while (!window_->shouldClose()) {
        Input::get().beginFrame();
        window_->pollEvents();


        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        Time::get().tick();
        window_->setTitle(std::format("Quincunx - {:.3f}ms ({:3.0f}fps)", Time::get().deltaTimeMs(), 1.0f / Time::get().deltaTime()));
        tick();

        ImGui::ShowDemoWindow();



        draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window_->swapBuffers();
    }
    return 0;
}
