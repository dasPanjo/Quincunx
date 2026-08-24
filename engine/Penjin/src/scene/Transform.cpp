#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
#include <stdexcept>

namespace Penjin {

Transform::Transform(GameObject* gameObject)
    : gameObject_(gameObject)
{
    if (!gameObject_) {
        throw std::invalid_argument("Transform requires a valid GameObject");
    }
}

Transform::~Transform()
{
    detachFromParent();

    for (Transform* child : children_) {
        child->parent_ = nullptr;
    }

    children_.clear();
}

void Transform::rotate(glm::vec3 vec) {
    glm::quat delta = glm::quat(vec);
    localRotation *= delta;
}

glm::mat4 Transform::localMatrix() const
{
    return glm::translate(glm::mat4(1.0f), localPosition)
         * glm::mat4_cast(localRotation)
         * glm::scale(glm::mat4(1.0f), localScale);
}

glm::mat4 Transform::worldMatrix() const
{
    if (parent_) {
        return parent_->worldMatrix() * localMatrix();
    }

    return localMatrix();
}

glm::vec3 Transform::worldPosition() const
{
    return glm::vec3(worldMatrix()[3]);
}

glm::quat Transform::worldRotation() const
{
    if (parent_) {
        return glm::normalize(parent_->worldRotation() * localRotation);
    }

    return glm::normalize(localRotation);
}

glm::vec3 Transform::worldScale() const
{
    const glm::mat4 matrix = worldMatrix();

    return {
        glm::length(glm::vec3(matrix[0])),
        glm::length(glm::vec3(matrix[1])),
        glm::length(glm::vec3(matrix[2]))
    };
}
    void Transform::setParent(Transform* parent, bool worldPositionStays)
{
    if (parent == this) {
        throw std::invalid_argument(
            "A Transform cannot be its own parent"
        );
    }

    for (Transform* p = parent; p; p = p->parent_) {
        if (p == this) {
            throw std::invalid_argument(
                "Cannot parent a Transform to its descendant"
            );
        }
    }

    if (parent_ == parent) {
        return;
    }

    glm::mat4 world = worldMatrix();

    detachFromParent();

    parent_ = parent;

    if (parent_) {
        parent_->children_.push_back(this);
    }

    if (!worldPositionStays) {
        return;
    }

    const glm::mat4 parentInverse =
        parent_
            ? glm::inverse(parent_->worldMatrix())
            : glm::mat4(1.0f);

    const glm::mat4 local = parentInverse * world;

    localPosition = glm::vec3(local[3]);

    glm::vec3 scale{
        glm::length(glm::vec3(local[0])),
        glm::length(glm::vec3(local[1])),
        glm::length(glm::vec3(local[2]))
    };

    localScale = scale;

    glm::mat3 rotationMatrix{
        glm::vec3(local[0]) / scale.x,
        glm::vec3(local[1]) / scale.y,
        glm::vec3(local[2]) / scale.z
    };

    localRotation = glm::normalize(
        glm::quat_cast(rotationMatrix)
    );
}

void Transform::detachFromParent()
{
    if (!parent_) {
        return;
    }

    auto& siblings = parent_->children_;

    siblings.erase(
        std::remove(siblings.begin(), siblings.end(), this),
        siblings.end()
    );

    parent_ = nullptr;
}

} // namespace Penjin