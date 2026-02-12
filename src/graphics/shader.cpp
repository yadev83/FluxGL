#include <fluxgl/graphics/shader.h>
#include <fluxgl/core/error.h>
#include <fluxgl/utils/file.h>

#include <fluxgl/core/log.h>

#include <glad/glad.h>

namespace fluxgl {
    void Shader::compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
        int success;
        
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSourceCStr = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
        glCompileShader(vertexShader);
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

        mID = glCreateProgram();
        glAttachShader(mID, vertexShader);
        glAttachShader(mID, fragmentShader);
        glLinkProgram(mID);
        glGetProgramiv(mID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(mID, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Shader linking failed: " + std::string(infoLog)};
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
        glDeleteProgram(mID);
    }

    void Shader::bind() const {
        glUseProgram(mID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(mID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    Shader Shader::fromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
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