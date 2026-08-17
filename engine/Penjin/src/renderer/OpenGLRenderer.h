#pragma once
#include "IRenderer.h"

namespace Penjin {
    class OpenGLRenderer : public IRenderer {
    public:
        bool init() override;
        void cleanup() override;

        void beginFrame(const glm::vec4 &clearColor) override;
        void endFrame() override;

        void setViewProjection(const glm::mat4& view, const glm::mat4& projection) override;
        void drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix) override;
    };
}
