#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class HelloApp : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Mesh triangle;
        fluxgl::Material material;

    protected:
        void onInit() override {
            material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            triangle = fluxgl::Mesh::fromVertices({ 
                {.position = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}}, 
                {.position = {0.0f, 0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
                {.position = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}}
            });
        }

        void onRender() override {
            fluxgl::Renderer::clear({0.2f, 0.3f, 0.3f});
            fluxgl::Renderer::draw(triangle, material);
        }
};

int main() {
    HelloApp app(800, 600, "Hello FluxGL");
    app.run();

    return 0;
}