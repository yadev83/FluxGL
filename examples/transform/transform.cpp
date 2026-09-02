#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

class TransformExample : public fluxgl::Scene {
    fluxgl::Entity entity, camera;

    public:
        void onLoad() override {
            auto& resources = getContext().resourceManager;
            auto& vfs = getContext().vfs;

            resources.addResource<fluxgl::Shader>("shader", fluxgl::Shader::loadFromSource(
                vfs.readText("assets/shaders/vertex.glsl"), 
                vfs.readText("assets/shaders/fragment.glsl")
            ));
            resources.addResource<fluxgl::Texture>("container", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/container.jpg")));
            resources.addResource<fluxgl::Texture>("awesomeface", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/awesomeface.png")));
            resources.addResource<fluxgl::Mesh>("mesh", fluxgl::Mesh::quad());
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
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_F11)) {
                getContext().window.setWindowFullscreenMode(!getContext().window.getWindowFullscreenMode());
            }
            
            entity.getComponent<fluxgl::Transform>().rotation.z += 20.0f * deltaTime; // Rotate around Z-axis

            fluxgl::Renderer::beginFrame();

            auto& cameraComponent = camera.getComponent<fluxgl::Camera>();
            auto& cameraTransform = camera.getComponent<fluxgl::Transform>();

            auto& meshTransform = entity.getComponent<fluxgl::Transform>();
            auto& meshRenderer = entity.getComponent<fluxgl::MeshRenderer>();

            fluxgl::Renderer::setCamera(cameraComponent.getViewMatrix(cameraTransform), cameraComponent.getProjectionMatrix(), cameraTransform.position);
            fluxgl::Renderer::drawMesh(
                getContext().resourceManager.getResource<fluxgl::Mesh>(meshRenderer.mesh),
                meshTransform.getModelMatrix(),
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
    fluxgl::App app(800, 600, "Transform Example");

    fluxgl::SceneManager::get().registerScene<TransformExample>("Transform");
    app.run();

    return 0;
}