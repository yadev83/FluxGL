#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

int main() {
    try {
        fluxgl::Logger::init("hello");
        fluxgl::Window window = fluxgl::Window(800, 600, "Hello FluxGL");

        const char *vertexShaderSource = R"(
            #version 460 core
            layout (location = 0) in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 460 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }
        )";

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw fluxgl::Error{fluxgl::ErrorCode::ShaderCompilationFailed, "Vertex shader compilation failed: " + std::string(infoLog)};
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw fluxgl::Error{fluxgl::ErrorCode::ShaderCompilationFailed, "Fragment shader compilation failed: " + std::string(infoLog)};
        }

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            throw fluxgl::Error{fluxgl::ErrorCode::ShaderCompilationFailed, "Shader program linking failed: " + std::string(infoLog)};
        }

        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        fluxgl::Mesh rectangle = fluxgl::Mesh::quad();

        while (!window.shouldClose()) {
            window.pollEvents();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            rectangle.draw();

            window.swapBuffers();
        }
    } catch (const fluxgl::Error& error) {
        std::stringstream oss;
        oss << "Error: " << error.code << " - " << error.message;
        FLUXGL_LOG_ERROR(oss.str());
        return 1;
    } catch (const std::exception& e) {
        FLUXGL_LOG_ERROR("std::exception: " + std::string(e.what()));
        return 1;
    } catch (...) {
        FLUXGL_LOG_ERROR("Unknown error occurred");
        return 1;
    }

    return 0;
}