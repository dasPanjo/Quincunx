#include "Input.h"

#include <optional>

namespace {
    std::optional<Penjin::MouseButton> toMouseButton(Uint8 button) {
        switch (button) {
            case SDL_BUTTON_LEFT: return Penjin::MouseButton::Left;
            case SDL_BUTTON_RIGHT: return Penjin::MouseButton::Right;
            case SDL_BUTTON_MIDDLE: return Penjin::MouseButton::Middle;
            default: return std::nullopt;
        }
    }
}

Penjin::Input& Penjin::Input::get() {
    static Input instance;
    return instance;
}

void Penjin::Input::beginFrame() {
    previousKeys_ = currentKeys_;
    previousMouseButtons_ = currentMouseButtons_;
    mouseDelta_ = glm::ivec2{0, 0};
    scrollDelta_ = 0;
}

void Penjin::Input::processEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            const auto key = fromScancode(event.key.scancode);
            currentKeys_[static_cast<std::size_t>(key)] = (event.type == SDL_EVENT_KEY_DOWN);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (const auto button = toMouseButton(event.button.button)) {
                currentMouseButtons_[static_cast<std::size_t>(*button)] = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            const glm::ivec2 newPosition{event.motion.x, event.motion.y};
            mouseDelta_ += newPosition - mousePosition_;
            mousePosition_ = newPosition;
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            scrollDelta_ += event.wheel.y;
            break;
        }
        default:
            break;
    }
}

bool Penjin::Input::isKeyDown(KeyCode key) const {
    return currentKeys_[static_cast<std::size_t>(key)];
}

bool Penjin::Input::isKeyPressed(KeyCode key) const {
    const auto index = static_cast<std::size_t>(key);
    return currentKeys_[index] && !previousKeys_[index];
}

bool Penjin::Input::isKeyReleased(KeyCode key) const {
    const auto index = static_cast<std::size_t>(key);
    return !currentKeys_[index] && previousKeys_[index];
}

bool Penjin::Input::isMouseButtonDown(MouseButton button) const {
    return currentMouseButtons_[static_cast<std::size_t>(button)];
}

glm::ivec2 Penjin::Input::mousePosition() const {
    return mousePosition_;
}

glm::ivec2 Penjin::Input::mouseDelta() const {
    return mouseDelta_;
}

int Penjin::Input::scrollDelta() const {
    return scrollDelta_;
}
