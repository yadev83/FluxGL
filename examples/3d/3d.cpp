#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#include "fps_controller.h"

#define MAX_ENTITIES 10

class Scene3D : public fluxgl::Scene {
    private:
        fluxgl::Entity camera;
        std::vector<fluxgl::Entity> entities;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();

            auto& resources = context->resourceManager;
            auto& vfs = context->vfs;

            resources.addShader("shader", fluxgl::Shader::loadFromSource(vfs.readText("assets/shaders/vertex.glsl"), vfs.readText("assets/shaders/fragment.glsl")));
            resources.addTexture("container", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/container.jpg")));
            resources.addTexture("awesomeface", fluxgl::Texture::loadFromMemory(vfs.read("assets/textures/awesomeface.png")));
            resources.addMesh("cube", fluxgl::Mesh::cube());
            resources.addMesh("sphere", fluxgl::Mesh::sphere());
        }

        void onInit() override {
            for(int i = 0; i < MAX_ENTITIES; i++) {
                entities.emplace_back(createEntity());
                auto& entity = entities.back();

                auto& transform = entity.addComponent<fluxgl::Transform>();
                auto& meshRenderer = entity.addComponent<fluxgl::MeshRenderer>();
                
                bool isEven = i % 2 == 0;

                meshRenderer.material.shader = "shader";
                meshRenderer.material.albedoTextures.push_back("container");
                if(isEven) meshRenderer.material.albedoTextures.push_back("awesomeface");
                
                meshRenderer.mesh = isEven ? "sphere" : "cube";
                // Randomize position
                transform.position = glm::vec3((float)rand() / (float)RAND_MAX * 6.0f - 3.0f, (float)rand() / (float)RAND_MAX * 6.0f - 3.0f, (float)rand() / (float)RAND_MAX * 6.0f - 3.0f);
            }

            camera = createEntity();
            camera.addComponent<fluxgl::Camera>();
            auto& cameraTransform = camera.addComponent<fluxgl::Transform>();
            cameraTransform.position = {0.0f, 0.0f, 3.0f};
            camera.registerBehavior<FirstPersonController>();
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) context->window.isMouseLocked() ? context->window.setMouseLocked(false) : context->window.setWindowShouldClose();
            if(context->inputManager.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) context->window.setMouseLocked(true);

            for(fluxgl::Entity& entity : entities) {
                auto& transform = entity.getComponent<fluxgl::Transform>();
                transform.rotation.x += 20.0f * deltaTime; // Rotate around X-axis
                transform.rotation.z += 20.0f * deltaTime; // Rotate around Z-axis
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "3D Example");
    
    fluxgl::SceneManager::get().registerScene<Scene3D>("3D");
    app.run();

    return 0;
}