
#include "GameObject.h"

#include <string>
#include <utility>

#include "Component.h"

Penjin::GameObject::GameObject(std::string name) :
    name_(std::move(name)), transform_(this) {

}

Penjin::GameObject::~GameObject() = default;

void Penjin::GameObject::tick() {
    for (const auto& component : this->components_) {
        component->tick();
    }
    for (auto t : transform_.children()) {
        t->gameObject().tick();
    }
}

void Penjin::GameObject::draw(Penjin::IRenderer &renderer) const {
    for (const auto& component : this->components_) {
        component->draw(renderer);
    }
    for (auto t : transform_.children()) {
        t->gameObject().draw(renderer);
    }
}
