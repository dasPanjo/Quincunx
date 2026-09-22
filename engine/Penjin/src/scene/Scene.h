#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "../renderer/IRenderer.h"


namespace Penjin {
    class Scene final {
    public:
        GameObject& createGameObject(std::string name = "GameObject");
        void tick();
        void draw(IRenderer& renderer) const;
        [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>& gameObjects() const { return gameObjects_; }
        Camera* mainCamera;
    private:
        std::vector<std::unique_ptr<GameObject>> gameObjects_;
    };
}
