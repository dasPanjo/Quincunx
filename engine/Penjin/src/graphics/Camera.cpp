
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"

glm::mat4 Penjin::Camera::viewMatrix() const {
    const Transform &transform = gameObject().transform();
    glm::mat4 cameraWorld = glm::translate(glm::mat4(1.0f), transform.worldPosition())  * glm::mat4_cast(transform.worldRotation());
    return glm::inverse(cameraWorld);
}

glm::mat4 Penjin::Camera::projectionMatrix() const {
    glm::ivec2 windowSize = Application::get().window().windowSize();
    float aspect = (float)windowSize.x / (float)windowSize.y;
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Penjin::Camera::tick() {
    Component::tick();
}
