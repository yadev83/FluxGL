#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#include "behaviors/fps_controller.h"

class Example : public fluxgl::Scene {
    private:
        fluxgl::Entity camera;

        fluxgl::Entity cubeA;
        fluxgl::Entity cubeB;
        fluxgl::Entity cubeC;
        
        fluxgl::Entity ambientLight;
        fluxgl::Entity sunLight;
        fluxgl::Entity pointLightA;
        fluxgl::Entity pointLightB;

    public:
        void onLoad() override {
            const glm::vec3 ambientLightColor = {0.6f, 0.2f, 0.2f};
            registerSystem<fluxgl::RenderSystem>();
            fluxgl::Renderer::setClearColor(0.4f * ambientLightColor);

            camera = createEntity();
            camera.registerBehavior<FirstPersonController>();
            camera.addComponent<fluxgl::Camera>();
            auto& cameraTransform = camera.addComponent<fluxgl::Transform>();
            cameraTransform.position = {0.0f, 0.0f, 5.0f};

            cubeA = createEntity();
            cubeA.addComponent<fluxgl::Transform>();
            auto& cubeARenderer = cubeA.addComponent<fluxgl::MeshRenderer>();
            cubeARenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            cubeARenderer.material.albedoColor = {1.0f, 0.2f, 0.2f};
            cubeARenderer.mesh = fluxgl::Mesh::cube();

            cubeB = createEntity();
            auto& cubeBTransform = cubeB.addComponent<fluxgl::Transform>();
            cubeBTransform.position = {2.0f, 0.0f, -2.0f};
            auto& cubeBRenderer = cubeB.addComponent<fluxgl::MeshRenderer>();
            cubeBRenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            cubeBRenderer.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.png"));
            cubeBRenderer.material.specularMap = fluxgl::Texture::loadFromFile("assets/textures/container_specular.png");
            cubeBRenderer.mesh = fluxgl::Mesh::cube();

            cubeC = createEntity();
            auto& cubeCTransform = cubeC.addComponent<fluxgl::Transform>();
            cubeCTransform.position = {2.0f, 0.0f, 2.0f};
            auto& cubeCRenderer = cubeC.addComponent<fluxgl::MeshRenderer>();
            cubeCRenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            cubeCRenderer.material.albedoColor = glm::vec3(0.0f);
            cubeCRenderer.material.specularColor = glm::vec3(0.0f);
            cubeCRenderer.material.emissionColor = glm::vec3(1.0f);
            cubeCRenderer.material.emissionMap = fluxgl::Texture::loadFromFile("assets/textures/emission_map.jpg");
            cubeCRenderer.mesh = fluxgl::Mesh::cube();

            ambientLight = createEntity();
            auto& ambientLightComponent = ambientLight.addComponent<fluxgl::Light>();
            ambientLightComponent.color = ambientLightColor;
            ambientLightComponent.intensity = 0.1f;

            sunLight = createEntity();
            auto& sunLightTransform = sunLight.addComponent<fluxgl::Transform>();
            sunLightTransform.rotation = {-0.2f, -1.0f, -3.0f};
            auto& sunLightComponent = sunLight.addComponent<fluxgl::Light>();
            sunLightComponent.type = fluxgl::LightType::Directional;
            sunLightComponent.color = ambientLightColor;
            sunLightComponent.intensity = 0.5f;

            pointLightA = createEntity();
            auto& pointLightATransform = pointLightA.addComponent<fluxgl::Transform>();
            pointLightATransform.position = {3.0f, 5.0f, 2.0f};
            auto& pointLightARenderer = pointLightA.addComponent<fluxgl::MeshRenderer>();
            pointLightARenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/flat.frag.glsl");
            pointLightARenderer.material.albedoColor = {1.0f, 1.0f, 1.0f};
            pointLightARenderer.mesh = fluxgl::Mesh::sphere();
            auto& pointLightAComponent = pointLightA.addComponent<fluxgl::Light>();
            pointLightAComponent.type = fluxgl::LightType::Point;
            pointLightAComponent.color = {1.0f, 1.0f, 1.0f};
            pointLightAComponent.intensity = 1.0f;

            pointLightB = createEntity();
            auto& pointLightBTransform = pointLightB.addComponent<fluxgl::Transform>();
            pointLightBTransform.position = {0.0f, 0.0f, -2.0f};
            pointLightBTransform.scale = {0.2f, 0.2f, 0.2f};
            auto& pointLightBRenderer = pointLightB.addComponent<fluxgl::MeshRenderer>();
            pointLightBRenderer.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/flat.frag.glsl");
            pointLightBRenderer.material.albedoColor = {0.0f, 0.0f, 1.0f};
            pointLightBRenderer.mesh = fluxgl::Mesh::sphere();
            auto& pointLightBComponent = pointLightB.addComponent<fluxgl::Light>();
            pointLightBComponent.type = fluxgl::LightType::Point;
            pointLightBComponent.color = {0.0f, 0.0f, 1.0f};
            pointLightBComponent.intensity = 1.0f;
        }

        void onInit() override {
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) context->window.isMouseLocked() ? context->window.setMouseLocked(false) : context->window.setWindowShouldClose();
            if(context->inputManager.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) context->window.setMouseLocked(true);

            // Animate point light around circle
            static float time = 0.0f; // animation elapsed time
            time += deltaTime;

            // Animation settings
            float radius = 2.0f;
            glm::vec3 center = cubeA.getComponent<fluxgl::Transform>().position;
            float speed = 3.0f;

            // Update position
            auto& lightTransform = pointLightB.getComponent<fluxgl::Transform>();
            lightTransform.position.y = center.y + radius * sin(time * speed);
            lightTransform.position.z = center.z + radius * cos(time * speed);
        }
};

int main() {
    fluxgl::App app(800, 600, "Lighting Example");
    fluxgl::SceneManager::get().registerScene<Example>("Main");
    app.run();

    return 0;
}