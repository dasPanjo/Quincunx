#include "Window.h"

Penjin::Window::Window(int width, int height, const std::string &title) {
    // Init SDL
    // Init Glad
    std::cout << "Penjin::Window::Window()" << std::endl;
}

Penjin::Window::~Window() {
}

void Penjin::Window::pollEvents() {
}

bool Penjin::Window::shouldClose() const {
    return true;
}

bool Penjin::Window::swapBuffers() {
}
