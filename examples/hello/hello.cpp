#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class HelloApp : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Renderable entity;

    protected:
        void onInit() override {
            entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            entity.mesh = fluxgl::Mesh::fromVertices({ 
                {.position = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}}, 
                {.position = {0.0f, 0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
                {.position = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}}
            });
        }

        void onUpdate(float deltaTime) override {
            if(getInput().isKeyPressed(GLFW_KEY_ESCAPE)) {
                getWindow().quit();
            }
        }

        void onRender() override {
            fluxgl::Renderer::clear({0.2f, 0.3f, 0.3f});
            fluxgl::Renderer::draw(entity);
        }
};

int main() {
    HelloApp app(800, 600, "Hello FluxGL");
    app.run();

    return 0;
}