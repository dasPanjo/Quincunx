
#pragma once
#include <memory>
#include <vector>

#include "Model.h"
#include "../scene/Component.h"
#include "../renderer/IRenderer.h"

namespace Penjin {
    class MeshRenderer : public Component {
    public:
        void start() override;
        void draw(IRenderer &renderer) const override;
        std::shared_ptr<Model> model_;
        std::vector<std::shared_ptr<Material>> materials_;
    };
}
