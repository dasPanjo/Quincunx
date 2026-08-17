
#include "Material.h"

Penjin::Material::Material(std::shared_ptr<Shader> shader) : shader_(shader) {
}

void Penjin::Material::bind() const {
    shader_->bind();
}
