#pragma once

#include <string>
#include <glm/glm.hpp>

namespace fluxgl {
    class Shader {
        private:
            unsigned int mID = 0;
            void compile(const std::string& vertexSrc, const std::string& fragmentSrc);

        public:
            Shader() = default;
            ~Shader();

            void bind() const;
            void unbind() const;

            // helpers
            void setUniform(const std::string& name, int value) const;
            void setUniform(const std::string& name, float value) const;
            void setUniform(const std::string& name, const glm::vec3& value) const;
            void setUniform(const std::string& name, const glm::mat4& value) const;

            // builders
            static Shader fromFiles(const std::string& vertexPath, const std::string& fragmentPath);
            static Shader fromSource(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}