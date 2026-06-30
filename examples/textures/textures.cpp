#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Texture : public fluxgl::Scene {
    fluxgl::Entity entity;


    public:
        void onLoad() override {
            // Load resources
            auto& resources = getContext().resourceManager;
            auto& vfs = getContext().vfs;

            resources.addResource<fluxgl::Shader>("shader", fluxgl::Shader::loadFromSource(
                vfs.readText("assets/shaders/texture.vert"), 
                vfs.readText("assets/shaders/texture.frag")
            ));
            resources.addResource<fluxgl::Texture>("container", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/container.jpg")));
            resources.addResource<fluxgl::Texture>("awesomeface", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/awesomeface.png")));
            resources.addResource<fluxgl::Mesh>("mesh", fluxgl::Mesh::quad());
        }

        void onInit() override {
            auto& resources = getContext().resourceManager;

            entity = createEntity();
            auto& meshRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            meshRenderer.material.shader = "shader";
            meshRenderer.material.albedoTextures.push_back("container");
            meshRenderer.material.albedoTextures.push_back("awesomeface");

            meshRenderer.mesh = "mesh";
        }

        void onUpdate(float deltaTime) override {
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }

            auto& meshRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            fluxgl::Renderer::beginFrame();
            fluxgl::Renderer::drawMesh(
                getContext().resourceManager.getResource<fluxgl::Mesh>(meshRenderer.mesh),
                glm::mat4(1.0f),
                getContext().resourceManager.getResource<fluxgl::Shader>(meshRenderer.material.shader),
                getContext().resourceManager.getResources<fluxgl::Texture>(meshRenderer.material.albedoTextures),
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