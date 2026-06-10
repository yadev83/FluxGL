#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Texture : public fluxgl::Scene {
    fluxgl::Entity entity;


    public:
        void onLoad() override {
            // Load resources
            auto& resources = context->resourceManager;
            auto& vfs = context->vfs;

            resources.addShader("shader", fluxgl::Shader::loadFromSource(
                vfs.readText("assets/shaders/texture.vert"), 
                vfs.readText("assets/shaders/texture.frag")
            ));
            resources.addTexture("container", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/container.jpg")));
            resources.addTexture("awesomeface", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/awesomeface.png")));
            resources.addMesh("mesh", fluxgl::Mesh::quad());
        }

        void onInit() override {
            auto& resources = context->resourceManager;

            entity = createEntity();
            auto& meshRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            meshRenderer.material.shader = "shader";
            meshRenderer.material.albedoTextures.push_back("container");
            meshRenderer.material.albedoTextures.push_back("awesomeface");

            meshRenderer.mesh = "mesh";
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