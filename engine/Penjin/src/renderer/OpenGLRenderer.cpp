#include "OpenGLRenderer.h"

#include <format>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "../logger/Logger.h"

bool Penjin::OpenGLRenderer::init() {

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

void Penjin::OpenGLRenderer::cleanup() {
}

void Penjin::OpenGLRenderer::beginFrame(const glm::vec4 &clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Penjin::OpenGLRenderer::endFrame() {
}
