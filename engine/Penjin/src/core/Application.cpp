#include "Application.h"

#include <glad/glad.h>
#include <glm/vec4.hpp>

Penjin::Application::Application() {

}

Penjin::Application::~Application() {

}

int Penjin::Application::run() {
    window = new Window(800, 600, "Quincunx");

    // Trivial GLM usage to confirm it's wired up correctly.
    glm::vec4 clearColor(0.15f, 0.35f, 0.55f, 1.0f);

    while (!window->shouldClose()) {
        window->pollEvents();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);


        window->swapBuffers();
    }
    delete window;
    return 0;
}
