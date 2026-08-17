#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"
#include "../renderer/IRenderer.h"


namespace Penjin {
    class Scene final {
    public:
        GameObject& createGameObject(std::string name = "GameObject");
        void tick();
        void draw(IRenderer& renderer) const;
        [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>& gameObjects() const { return gameObjects_; }

    private:
        std::vector<std::unique_ptr<GameObject>> gameObjects_;
    };
}
