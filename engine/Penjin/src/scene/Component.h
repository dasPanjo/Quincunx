#pragma once

namespace Penjin {
    class GameObject;
    class Transform;
    class IRenderer;

    class Component {
        friend class GameObject;
    public:
        virtual ~Component() = default;

        [[nodiscard]] GameObject& gameObject() const { return *gameObject_; }
        [[nodiscard]] Transform& transform() const;

        virtual void start() {}
        virtual void tick() {}
        virtual void draw(IRenderer &renderer) const {}
    protected:
        Component() = default;

    private:
        void attachTo(GameObject* owner) {gameObject_ = owner;}
        GameObject* gameObject_ = nullptr;
    };
}