
#pragma once
#include <glm/glm.hpp>

#include "Shader.h"

namespace Penjin {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };


    class Mesh {
    public:
        Mesh();
        ~Mesh();
        bool load();
        void draw() const;
    private:
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;

        Shader shader_;

        // OpenGL object handles
        unsigned int VAO_ = 0;
        unsigned int VBO_ = 0;
        unsigned int EBO_ = 0;

    };
}
