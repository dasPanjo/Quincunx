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

        [[nodiscard]] bool isKeyDown(KeyCode key) const;
        [[nodiscard]] bool isKeyPressed(KeyCode key) const;
        [[nodiscard]] bool isKeyReleased(KeyCode key) const;
        [[nodiscard]] bool isMouseButtonDown(MouseButton button) const;

        [[nodiscard]] glm::ivec2 mousePosition() const {return mousePosition_;}
        [[nodiscard]] glm::vec2 mousePositionRelative() const{return mousePositionRelative_;}
        [[nodiscard]] glm::ivec2 mouseDelta() const{ return mouseDelta_;}
        [[nodiscard]] int scrollDelta() const {return scrollDelta_;}

    private:
        std::array<bool, KeyCodeCount> currentKeys_{};
        std::array<bool, KeyCodeCount> previousKeys_{};

        std::array<bool, MouseButtonCount> currentMouseButtons_{};
        std::array<bool, MouseButtonCount> previousMouseButtons_{};

        glm::ivec2 mousePosition_{};
        glm::vec2 mousePositionRelative_{};
        glm::ivec2 mouseDelta_{};
        int scrollDelta_ = 0;
    };
}
