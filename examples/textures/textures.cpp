#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Texture : public fluxgl::Scene {
    fluxgl::Entity entity;

    public:
        void onInit() override {
            entity = createEntity();
            auto& meshRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            meshRenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/texture.vert", "assets/shaders/texture.frag");
            meshRenderer.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            meshRenderer.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            
            meshRenderer.mesh = fluxgl::Mesh::quad();
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            auto& meshRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            fluxgl::Renderer::beginFrame();
            fluxgl::Renderer::drawMesh(meshRenderer.mesh, meshRenderer.material);
        }
};

int main() {
    fluxgl::App app(800, 600, "Texture Example");

    fluxgl::SceneManager::get().registerScene<Texture>("Texture");
    app.run();

    return 0;
}