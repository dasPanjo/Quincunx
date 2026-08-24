
#include "GameObject.h"

#include <imgui.h>
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

    ImGui::Begin(name().c_str());

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Position: %.2f, %.2f, %.2f", transform_.localPosition.x, transform_.localPosition.y, transform_.localPosition.z);
        glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(transform_.localRotation));
        ImGui::Text("Rotation: %.2f, %.2f, %.2f", eulerRotation.x, eulerRotation.y, eulerRotation.z);
        ImGui::Text("Scale:    %.2f, %.2f, %.2f", transform_.localScale.x, transform_.localScale.y, transform_.localScale.z);
    }

    for (const auto& component : this->components_) {
        component->draw(renderer);
    }
    for (auto t : transform_.children()) {
        t->gameObject().draw(renderer);
    }

    ImGui::End();
}
