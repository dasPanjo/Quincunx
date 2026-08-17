
#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace Penjin {
    class GameObject;

    class Transform final {
    public:
        explicit Transform(GameObject* gameObject);
        ~Transform();  // unlinks from parent, orphans children

        [[nodiscard]] GameObject& gameObject() const { return *gameObject_; }

        glm::vec3 localPosition{0.0f};
        glm::quat localRotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 localScale{1.0f};

        glm::mat4 localMatrix() const;
        glm::mat4 worldMatrix() const;
        glm::vec3 worldPosition() const;
        glm::quat worldRotation() const;
        glm::vec3 worldScale() const;      // "lossy" scale, same caveat as Unity's transform.lossyScale

        Transform* parent() const { return parent_; }
        const std::vector<Transform*>& children() const { return children_; }
        void setParent(Transform* parent, bool worldPositionStays = true);

    private:
        GameObject* gameObject_;
        Transform* parent_ = nullptr;
        std::vector<Transform*> children_;

        void detachFromParent();
    };
}
