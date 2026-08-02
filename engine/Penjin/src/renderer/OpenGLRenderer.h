#pragma once
#include "IRenderer.h"

namespace Penjin {
    class OpenGLRenderer : public IRenderer {
    public:
        bool init() override;
        void cleanup() override;
        void beginFrame(const glm::vec4 &clearColor) override;
        void endFrame() override;
    };
}
