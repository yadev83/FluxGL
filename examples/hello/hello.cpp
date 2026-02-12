#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

int main() {
    try {
        fluxgl::Logger::init("hello");
        fluxgl::Window window = fluxgl::Window(800, 600, "Hello FluxGL");
        
        fluxgl::Shader shader = fluxgl::Shader::fromFiles("vertex.glsl", "fragment.glsl");
        fluxgl::Mesh rectangle = fluxgl::Mesh::quad();

        while (!window.shouldClose()) {
            window.pollEvents();

            fluxgl::Renderer::clear({0.2f, 0.3f, 0.3f, 1.0f}); 
            fluxgl::Renderer::draw(rectangle, shader);

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