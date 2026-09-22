#pragma once
#include <glm/fwd.hpp>

#include "Component.h"

namespace Penjin {
    class Camera : public Component {
    public:
        void tick() override;
        [[nodiscard]] glm::mat4 viewMatrix() const;
    private:

    };
}
