#pragma once
#include <memory>
#include <vector>

#include "Material.h"
#include "Mesh.h"

namespace Penjin {
    class IRenderer;

    class Model {
    public:
        std::vector<std::shared_ptr<Penjin::Mesh>> meshes_;
        std::vector<std::shared_ptr<Penjin::Material>> materials_;

        bool loadDemo();
    };
}
