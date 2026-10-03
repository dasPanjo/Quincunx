#pragma once
#include <memory>
#include <glm/fwd.hpp>

namespace Penjin {
    class Shader final {
    public:
        static std::shared_ptr<Shader> createFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        static std::shared_ptr<Shader> createUnlitColor();

        ~Shader();

        void bind() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;

    private:
        unsigned int programId_ = 0;
    };
}