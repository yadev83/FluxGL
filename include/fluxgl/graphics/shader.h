#pragma once

#include <string>
#include <glm/glm.hpp>

namespace fluxgl {
    class Shader {
        private:
            unsigned int m_ID = 0;
            void compile(const std::string& vertexSrc, const std::string& fragmentSrc);

        public:
            Shader() = default;
            ~Shader();

            // Delete copy constructor and copy assignment operator to prevent copying 
            Shader(const Shader&) = delete; 
            Shader& operator=(const Shader&) = delete;

            // Instead, allow move
            Shader(Shader&& other) noexcept;
            Shader& operator=(Shader&& other) noexcept;

            bool isValid() const { return m_ID != 0; }
            void bind() const;
            void unbind() const;

            // helpers
            void setUniform(const std::string& name, bool value) const;
            void setUniform(const std::string& name, int value) const;
            void setUniform(const std::string& name, unsigned int value) const;
            void setUniform(const std::string& name, float value) const;
            void setUniform(const std::string& name, const glm::vec3& value) const;
            void setUniform(const std::string& name, const glm::mat4& value) const;

            // builders
            static Shader loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
            static Shader loadFromSource(const std::string& vertexSrc, const std::string& fragmentSrc);

            static Shader defaultLit();
            static Shader defaultUnlit();
    };
}