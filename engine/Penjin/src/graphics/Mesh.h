
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Penjin {
    class IRenderer;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };


    class Mesh {
    public:
        Mesh();
        ~Mesh();

        bool loadDemo();

        bool createVertexArray();

        [[nodiscard]] unsigned int vao() const { return VAO_; }
        [[nodiscard]] std::vector<Vertex> vertices() const { return vertices_; }
        [[nodiscard]] std::vector<unsigned int> indices() const { return indices_; }
        [[nodiscard]] unsigned int materialId() const { return materialId_; }

        std::string name_;

    private:
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        unsigned int materialId_ = 0;

        // OpenGL object handles
        unsigned int VAO_ = 0;
        unsigned int VBO_ = 0;
        unsigned int EBO_ = 0;

    };
}
