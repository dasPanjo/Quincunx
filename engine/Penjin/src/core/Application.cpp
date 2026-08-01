#include "Application.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/vec4.hpp>

#include "../logger/Logger.h"
#include "../logger/ConsoleSink.h"

#include <format>

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
        window_->pollEvents();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        time_.tick();

        tick();
        draw();

        window_->setTitle(std::format("Quincunx - {:.0f} fps", 1.0f / time_.deltaTime()));
        window_->swapBuffers();
    }
    return 0;
}
