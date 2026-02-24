#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class Hello : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

    public:
        void onInit() override {
            entity = createEntity();
            auto& entityRenderer = entity.addComponent<fluxgl::MeshRenderer>();

            entityRenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            entityRenderer.mesh = fluxgl::Mesh::fromVertices({ 
                {.position = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}}, 
                {.position = {0.0f, 0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
                {.position = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}}
            });
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            auto& entityRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            fluxgl::Renderer::beginFrame();
            fluxgl::Renderer::drawMesh(entityRenderer.mesh, entityRenderer.material);
        }
};

int main() {
    fluxgl::App app(800, 600, "Hello FluxGL");

    fluxgl::SceneManager::get().registerScene<Hello>("Hello");
    app.run();

    return 0;
}