
#include "Material.h"

#include <utility>

Penjin::Material::Material(std::shared_ptr<Shader> shader, std::string name) :
    shader_(std::move(shader)),
    name_(std::move(name)),
    baseColor_({1.0f, 1.0f, 1.0f, 1.0f}){
}

void Penjin::Material::bind() const {
    shader_->bind();
    shader_->setVec4("uColor", baseColor_);
}
