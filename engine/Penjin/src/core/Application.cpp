#include "Application.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/vec4.hpp>

Penjin::Application::Application() {

}

Penjin::Application::~Application() {

}

int Penjin::Application::run() {
    window_ = std::make_unique<Window>();
    if (!window_->createWindow("Quincunx", 800, 600)) {
        std::cerr << "Failed to create window!" << std::endl;
        return 1;
    }

    // Trivial GLM usage to confirm it's wired up correctly.
    glm::vec4 clearColor(0.15f, 0.35f, 0.55f, 1.0f);

    while (!window_->shouldClose()) {
        window_->pollEvents();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);


        window_->swapBuffers();
    }
    return 0;
}
