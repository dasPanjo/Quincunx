#include "Shader.h"

#include <format>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Logger.h"

std::shared_ptr<Penjin::Shader> Penjin::Shader::createFromSource(const std::string &vertexSource, const std::string &fragmentSource) {
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCode = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LOG_ERROR(std::format("Vertex shader compilation failed: {}", infoLog));
        return nullptr;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCode = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetProgramiv(fragmentShader, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(fragmentShader, 512, nullptr, infoLog);
        LOG_ERROR(std::format("Fragment shader linking failed: {}", infoLog));
        return nullptr;
    }

    const unsigned int programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        LOG_ERROR(std::format("Shader linking failed: {}", infoLog));
        return nullptr;
    }

    glUseProgram(programId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->programId_ = programId;
    return shader;
}

std::shared_ptr<Penjin::Shader> Penjin::Shader::createUnlitColor() {
    const std::string vertexSource =
        "#version 330 core \n"
        "layout(location = 0) in vec3 aPosition;"
        "layout(location = 1) in vec3 aNormal;"
        "layout(location = 2) in vec2 aUV;"

        "void main() { "
        "   gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0); "
        "}";

    const std::string fragmentSource =
        "#version 330 core \n"
        "out vec4 FragColor;"
        "uniform vec4 uColor;"
        "void main() { "
        "   FragColor = uColor; "
        "}";

    return createFromSource(vertexSource, fragmentSource);
}

Penjin::Shader::~Shader() {
}

void Penjin::Shader::bind() const {
    glUseProgram(programId_);
}


static int getUniformLocation(unsigned int programID, const std::string& name)
{
    int loc = glGetUniformLocation(programID, name.c_str());

    if (loc == -1)
        LOG_WARN(
            std::format("[SHADER] Uniform '{}' not found", name)
        );

    return loc;
}

void Penjin::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(getUniformLocation(programId_, name), value);
}

void Penjin::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(getUniformLocation(programId_,  name), value);
}

void Penjin::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(getUniformLocation(programId_, name), value);
}

void Penjin::Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(getUniformLocation(programId_, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Penjin::Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(getUniformLocation(programId_, name), 1, glm::value_ptr(value));
}
void Penjin::Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(getUniformLocation(programId_, name), 1, glm::value_ptr(value));
}
void Penjin::Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(getUniformLocation(programId_, name), 1, glm::value_ptr(value));
}
