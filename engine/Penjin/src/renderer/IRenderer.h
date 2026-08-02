#pragma once
#include <glm/vec4.hpp>

namespace Penjin {
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool init() = 0;
        virtual void cleanup() = 0;

        virtual void beginFrame(const glm::vec4& clearColor) = 0;
        virtual void endFrame() = 0;
    };
}
