#include "Model.h"

#include <format>

#include "Logger.h"

bool Penjin::Model::loadDemo() {
    meshes_.clear();
    std::unique_ptr<Penjin::Mesh> mesh = std::make_unique<Penjin::Mesh>();
    LOG_DEBUG(std::format("Mesh init: {}", mesh->loadDemo()));
    meshes_.push_back(std::move(mesh));
    std::shared_ptr<Shader> shader = Shader::createUnlitColor();
    if (shader == nullptr) {
        return false;
    }
    materials_.push_back(std::make_shared<Material>(shader, "Quad material"));
    return true;
}
