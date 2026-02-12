#include <fluxgl/graphics/shader.h>
#include <fluxgl/core/error.h>
#include <fluxgl/utils/file.h>

#include <fluxgl/core/log.h>

#include <glad/glad.h>

namespace fluxgl {
    void Shader::compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
        FLUXGL_LOG_DEBUG("Compiling shader program...");       
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSourceCStr = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
        glCompileShader(vertexShader);

        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Vertex shader compilation failed: " + std::string(infoLog)};
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSourceCStr = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Fragment shader compilation failed: " + std::string(infoLog)};
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Shader linking failed: " + std::string(infoLog)};
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        FLUXGL_LOG_DEBUG("Shader program linked (ID: " + std::to_string(m_ID) + ")");
    }

    Shader::~Shader() {
        if(m_ID) {
            FLUXGL_LOG_DEBUG("Deleting shader program (ID: " + std::to_string(m_ID) + ")");
            glDeleteProgram(m_ID);
        }
    }

    Shader::Shader(Shader&& other) noexcept {
        m_ID = other.m_ID; 
        other.m_ID = 0; // Prevent deletion of the moved-from shader
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if(m_ID) {
                glDeleteProgram(m_ID);
            }
            
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        return *this;
    }

    void Shader::bind() const {
        glUseProgram(m_ID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    Shader Shader::fromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
        FLUXGL_LOG_DEBUG("Loading shader from files: " + vertexPath + ", " + fragmentPath);
        std::string vertexSrc = readFileToString("assets/shaders/" + vertexPath);
        std::string fragmentSrc = readFileToString("assets/shaders/" + fragmentPath);
        return Shader::fromSource(vertexSrc, fragmentSrc);
    }

    Shader Shader::fromSource(const std::string& vertexSrc, const std::string& fragmentSrc) {
        Shader shader; 
        shader.compile(vertexSrc, fragmentSrc); 
        return shader; 
    }
}