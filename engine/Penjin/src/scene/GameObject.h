#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Transform.h"
#include "../renderer/IRenderer.h"

namespace Penjin {
    class Component;

    class GameObject final {
    public:
        explicit GameObject(std::string name = "GameObject");
        ~GameObject();

        [[nodiscard]] const std::string& name() const { return name_; }
        Transform& transform() { return transform_; }
        [[nodiscard]] const Transform& transform() const { return transform_; }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Penjin::Component");
            auto owned = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *owned;
            ref.attachTo(this);
            components_.push_back(std::move(owned));
            ref.start();
            return ref;
        }

        template <typename T>
        T* getComponent() const {
            static_assert(std::is_base_of_v<Component, T>,
                          "T must derive from Penjin::Component");

            for (const auto& component : components_) {
                if (auto* ptr = dynamic_cast<T*>(component.get())) {
                    return ptr;
                }
            }

            return nullptr;
        }

        template <typename T>
        std::vector<T*> getComponents() const {
            static_assert(std::is_base_of_v<Component, T>,
                          "T must derive from Penjin::Component");

            std::vector<T*> result;

            for (const auto& component : components_) {
                if (auto* ptr = dynamic_cast<T*>(component.get())) {
                    result.push_back(ptr);
                }
            }

            return result;
        }


        void tick();
        void draw(IRenderer& renderer) const;

    private:
        std::string name_;
        Transform transform_;                                   // destroyed after components_ (see below)
        std::vector<std::unique_ptr<Component>> components_;
    };
}
