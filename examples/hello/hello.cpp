#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class Hello : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

    public:
        void onLoad() override {
            // Load resources
            auto& resources = getContext().resourceManager;

            resources.addResource<fluxgl::Shader>("shader", fluxgl::Shader::loadFromSource(
                getContext().vfs.readText("assets/shaders/vertex.glsl"),
                getContext().vfs.readText("assets/shaders/fragment.glsl")
            ));
            resources.addResource<fluxgl::Mesh>("mesh", fluxgl::Mesh::fromVertices({ 
                {.position = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}}, 
                {.position = {0.0f, 0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
                {.position = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}}
            }));
        }

        void onInit() override {
            auto& resources = getContext().resourceManager;
            entity = createEntity();
            auto& entityRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            
            entityRenderer.material.shader = "shader";
            entityRenderer.mesh = "mesh";
        }

        void onUpdate(float deltaTime) override {
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }

            auto& entityRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            auto mesh = getContext().resourceManager.getResource<fluxgl::Mesh>(entityRenderer.mesh);
            auto shader = getContext().resourceManager.getResource<fluxgl::Shader>(entityRenderer.material.shader);
            auto albedoTextures = getContext().resourceManager.getResources<fluxgl::Texture>(entityRenderer.material.albedoTextures);
            auto normalMap = getContext().resourceManager.getResource<fluxgl::Texture>(entityRenderer.material.normalMap);
            auto specularMap = getContext().resourceManager.getResource<fluxgl::Texture>(entityRenderer.material.specularMap);
            auto emissionMap = getContext().resourceManager.getResource<fluxgl::Texture>(entityRenderer.material.emissionMap);

            fluxgl::Renderer::beginFrame();
            fluxgl::Renderer::drawMesh(
                mesh,
                glm::mat4(1.0f),
                
                shader,
                albedoTextures,
                normalMap,
                specularMap,
                emissionMap,

                entityRenderer.material.albedoColor,
                entityRenderer.material.specularColor,
                entityRenderer.material.emissionColor,
                entityRenderer.material.shininess
            );
        }
};

int main() {
    fluxgl::App app(800, 600, "Hello FluxGL");

    fluxgl::SceneManager::get().registerScene<Hello>("Hello");
    app.run();

    return 0;
}