#pragma once

#include <array>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "KeyCode.h"
#include "MouseButton.h"

namespace Penjin {
    class Input {
    public:
        static Input& get();

        void beginFrame();
        void processEvent(const SDL_Event& event);

        bool isKeyDown(KeyCode key) const;
        bool isKeyPressed(KeyCode key) const;
        bool isKeyReleased(KeyCode key) const;

        bool isMouseButtonDown(MouseButton button) const;
        glm::ivec2 mousePosition() const;
        glm::ivec2 mouseDelta() const;
        int scrollDelta() const;

    private:
        std::array<bool, KeyCodeCount> currentKeys_{};
        std::array<bool, KeyCodeCount> previousKeys_{};

        std::array<bool, MouseButtonCount> currentMouseButtons_{};
        std::array<bool, MouseButtonCount> previousMouseButtons_{};

        glm::ivec2 mousePosition_{};
        glm::ivec2 mouseDelta_{};
        int scrollDelta_ = 0;
    };
}
