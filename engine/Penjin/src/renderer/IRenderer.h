#pragma once

#include <glm/glm.hpp>
#include "Material.h"
#include "Mesh.h"

namespace Penjin {
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool init() = 0;
        virtual void cleanup() = 0;

        virtual void beginFrame(const glm::vec4& clearColor) = 0;
        virtual void endFrame() = 0;

        virtual void setViewProjection(const glm::mat4& view, const glm::mat4& projection) = 0;
        virtual void drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix) = 0;
    };
}
