#include "Scene.h"

Penjin::GameObject &Penjin::Scene::createGameObject(std::string name) {
    gameObjects_.push_back(std::make_unique<GameObject>(std::move(name)));
    return *gameObjects_.back();
}

void Penjin::Scene::tick() {
    for (auto &gameObject : gameObjects_) {
        if(gameObject->transform().parent() == nullptr)
            gameObject->tick();
    }
}

void Penjin::Scene::draw(IRenderer &renderer) const {
    for (auto &gameObject : gameObjects_) {
        if(gameObject->transform().parent() == nullptr)
            gameObject->draw(renderer);
    }
}
