#pragma once
#include <glm/fwd.hpp>

#include "Component.h"

namespace Penjin {
    class Camera : public Component {
    public:
        void tick() override;
        [[nodiscard]] glm::mat4 viewMatrix() const;
        [[nodiscard]] glm::mat4 projectionMatrix() const;
    private:
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

    };
}
