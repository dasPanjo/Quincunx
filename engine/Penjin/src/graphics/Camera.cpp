
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"

glm::mat4 Penjin::Camera::viewMatrix() const {
    glm::ivec2 windowSize = Application::get().window().windowSize();
    float aspect = (float)windowSize.x / (float)windowSize.y;
    glm::mat4 viewMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(gameObject().transform().worldPosition()));
    return viewMatrix;
}

void Penjin::Camera::tick() {
    Component::tick();
//    transform().localPosition.x += 10 * Time::get().deltaTime() * 0.01f;
    transform().localPosition.z = -10;
}
