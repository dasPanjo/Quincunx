#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Penjin {
    class Material {
    public:
        std::shared_ptr<Shader> shader_;

        Material(std::shared_ptr<Shader> shader, std::string name = "Material");
        virtual ~Material() = default;

        void bind() const;

        std::string name_;
        glm::vec4 baseColor_;

    };
}
