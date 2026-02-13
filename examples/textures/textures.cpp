#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

int main() {
    try {
        // Logger initialisation (based on build type)
        if(FLUXGL_DEBUG) fluxgl::Logger::init(fluxgl::LogLevel::Trace);
        else fluxgl::Logger::init(fluxgl::LogLevel::Info);

        fluxgl::Window window(800, 600, "Texture Example");

        // Load texture
        fluxgl::Mesh quad = fluxgl::Mesh::quad();
        fluxgl::Material material;
        material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/texture.vert", "assets/shaders/texture.frag");
        material.texture = fluxgl::Texture::loadFromFile("assets/textures/container.jpg");

        while (!window.shouldClose()) {
            window.pollEvents();

            fluxgl::Renderer::clear();

            // Draw your textured object here (e.g., a quad)
            fluxgl::Renderer::draw(quad, material);

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