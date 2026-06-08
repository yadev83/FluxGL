#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class Hello : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

    public:
        void onLoad() override {
            // Load resources
            auto& resources = context->resourceManager;

            resources.addShader("shader", fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
            resources.addMesh("mesh", fluxgl::Mesh::fromVertices({ 
                {.position = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}}, 
                {.position = {0.0f, 0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
                {.position = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}}
            }));
        }

        void onInit() override {
            auto& resources = context->resourceManager;
            entity = createEntity();
            auto& entityRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            
            entityRenderer.material.shader = resources.findShader("shader");
            entityRenderer.mesh = resources.findMesh("mesh");
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            auto& entityRenderer = entity.getComponent<fluxgl::MeshRenderer>();
            auto mesh = context->resourceManager.getMesh(entityRenderer.mesh);
            auto shader = context->resourceManager.getShader(entityRenderer.material.shader);
            auto albedoTextures = context->resourceManager.getTextures(entityRenderer.material.albedoTextures);
            auto normalMap = context->resourceManager.getTexture(entityRenderer.material.normalMap);
            auto specularMap = context->resourceManager.getTexture(entityRenderer.material.specularMap);
            auto emissionMap = context->resourceManager.getTexture(entityRenderer.material.emissionMap);

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