#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Texture : public fluxgl::Scene {
    fluxgl::Entity entity;


    public:
        void onLoad() override {
            // Load resources
            auto& resources = context->resourceManager;
            resources.addShader("shader", fluxgl::Shader::loadFromFiles("assets/shaders/texture.vert", "assets/shaders/texture.frag"));
            resources.addTexture("container", fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            resources.addTexture("awesomeface", fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            resources.addMesh("mesh", fluxgl::Mesh::quad());
        }

        void onInit() override {
            auto& resources = context->resourceManager;

            entity = createEntity();
            auto& meshRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            meshRenderer.material.shader = resources.findShader("shader");
            meshRenderer.material.albedoTextures.push_back(resources.findTexture("container"));
            meshRenderer.material.albedoTextures.push_back(resources.findTexture("awesomeface"));

            meshRenderer.mesh = resources.findMesh("mesh");
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            auto& meshRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            fluxgl::Renderer::beginFrame();
            fluxgl::Renderer::drawMesh(
                context->resourceManager.getMesh(meshRenderer.mesh),
                glm::mat4(1.0f),
                context->resourceManager.getShader(meshRenderer.material.shader),
                context->resourceManager.getTextures(meshRenderer.material.albedoTextures),
                nullptr, nullptr, nullptr,
                meshRenderer.material.albedoColor,
                meshRenderer.material.specularColor,
                meshRenderer.material.emissionColor,
                meshRenderer.material.shininess
            );
        }
};

int main() {
    fluxgl::App app(800, 600, "Texture Example");

    fluxgl::SceneManager::get().registerScene<Texture>("Texture");
    app.run();

    return 0;
}