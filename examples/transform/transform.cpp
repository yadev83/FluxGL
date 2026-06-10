#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

class TransformExample : public fluxgl::Scene {
    fluxgl::Entity entity, camera;

    public:
        void onLoad() override {
            auto& resources = context->resourceManager;
            resources.addShader("shader", fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
            resources.addTexture("container", fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            resources.addTexture("awesomeface", fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            resources.addMesh("mesh", fluxgl::Mesh::quad());
        }

        void onInit() override {
            entity = createEntity();
            camera = createEntity();

            auto& cameraTransform = camera.addComponent<fluxgl::Transform>();
            auto& cameraComponent = camera.addComponent<fluxgl::Camera>();

            auto& entityTransform = entity.addComponent<fluxgl::Transform>();
            auto& entityRenderer = entity.addComponent<fluxgl::MeshRenderer>();
            
            entityRenderer.material.shader = "shader";
            entityRenderer.material.albedoTextures.push_back("container");
            entityRenderer.material.albedoTextures.push_back("awesomeface");
            
            entityRenderer.mesh = "mesh";

            entityTransform.scale = {0.5f, 0.5f, 0.5f};
            entityTransform.rotation = glm::vec3(-55.0f, 0.0f, 0.0f);

            cameraTransform.position = {0.0f, 0.0f, 3.0f};
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            entity.getComponent<fluxgl::Transform>().rotation.z += 20.0f * deltaTime; // Rotate around Z-axis

            fluxgl::Renderer::beginFrame();

            auto& cameraComponent = camera.getComponent<fluxgl::Camera>();
            auto& cameraTransform = camera.getComponent<fluxgl::Transform>();

            auto& meshTransform = entity.getComponent<fluxgl::Transform>();
            auto& meshRenderer = entity.getComponent<fluxgl::MeshRenderer>();

            fluxgl::Renderer::setCamera(cameraComponent.getViewMatrix(cameraTransform), cameraComponent.getProjectionMatrix(), cameraTransform.position);
            fluxgl::Renderer::drawMesh(
                context->resourceManager.getMesh(meshRenderer.mesh),
                meshTransform.getModelMatrix(),
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
    fluxgl::App app(800, 600, "Transform Example");

    fluxgl::SceneManager::get().registerScene<TransformExample>("Transform");
    app.run();

    return 0;
}