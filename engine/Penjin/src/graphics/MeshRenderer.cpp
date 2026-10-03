#include "MeshRenderer.h"

#include <format>
#include <imgui.h>

#include "GameObject.h"
#include "Logger.h"

void Penjin::MeshRenderer::start() {
    Component::start();
}

void Penjin::MeshRenderer::draw(IRenderer &renderer) const {
    for (const auto& mesh : model_->meshes_) {
        const auto& materialId = mesh->materialId();
        std::shared_ptr<Material> material;
        if (materials_.size() > materialId &&
            materials_.at(materialId) != nullptr) {
            material = materials_.at(materialId);
        } else {
            material = model_->materials_.at(materialId);
        }
        renderer.drawMesh(*mesh, *material, gameObject().transform().worldMatrix());
    }


    // ImGUI
    if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        std::string matLabel = std::format("Materials: {}##mesh_render", materials_.size());
        if (ImGui::TreeNodeEx(matLabel.c_str())) {
            for (const auto& material : materials_) {
                ImGui::Text(material->name_.c_str());
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string modelMeshLabel = std::format("Meshes: {}##model_asset", model_->meshes_.size());
            if (ImGui::TreeNodeEx(modelMeshLabel.c_str())) {
                for (const auto& mesh : model_->meshes_) {
                    ImGui::Text(mesh->name_.c_str());
                }
                ImGui::TreePop();
            }

            std::string modelMatLabel = std::format("Materials: {}##model_asset", model_->materials_.size());

            if (ImGui::TreeNodeEx(modelMatLabel.c_str())) {
                for (const auto& material : model_->materials_) {
                    ImGui::Text(material->name_.c_str());
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

}
