
#include "Mesh.h"

#include <format>
#include <glad/glad.h>

#include "Logger.h"
#include "../debug/Profiler.h"

Penjin::Mesh::Mesh() {
    // Vertex 0
    vertices_.push_back({
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f)
    });

    // Vertex 1
    vertices_.push_back({
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(1.0f, 0.0f)
    });

    // Vertex 2
    vertices_.push_back({
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    });

    // Vertex 3
    vertices_.push_back({
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)
    });

    indices_ = {
        0, 1, 2,
        2, 3, 0
    };
}

Penjin::Mesh::~Mesh() {

}

bool Penjin::Mesh::load() {

    // 1) Ask OpenGL for object IDs (handles) for one VAO, one VBO, one EBO.
    //    These IDs reference driver-managed GPU objects.
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    // 2) Bind the VAO first.
    //    The VAO records vertex attribute layout state and the EBO binding.
    glBindVertexArray(VAO_);

    // 3) Upload per-vertex data into GPU memory (array buffer).
    //    GL_STATIC_DRAW tells the driver this data changes rarely.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices_.size() * sizeof(Vertex)),
                 vertices_.data(),
                 GL_STATIC_DRAW);

    // 4) Upload index data into the element buffer.
    //    Indices let us reuse vertices and draw via glDrawElements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices_.size() * sizeof(unsigned int)),
                 indices_.data(),
                 GL_STATIC_DRAW);

    // 5) Describe attribute layout for the currently bound VAO.
    //    Attribute 0 maps to layout(location = 0) in the vertex shader.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );

    // Attribute 1 maps to the normal field in our interleaved Vertex struct.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal))
    );

    // Attribute 2 maps to UV coordinates used for texturing later.
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Penjin::Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, texCoords))
    );

    // 6) Unbind VAO to keep state transitions explicit and avoid accidental edits.
    glBindVertexArray(0);

    return true;
}

void Penjin::Mesh::draw() const {
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    Profiler::vertexCount+= vertices_.size();
    Profiler::drawCalls++;
}
