#pragma once
#include <memory>

#include "Shader.h"

namespace Penjin {
    class Material {
    public:
        std::shared_ptr<Shader> shader_;

        Material(std::shared_ptr<Shader> shader);
        virtual ~Material() = default;

        void bind() const;

    };
}
