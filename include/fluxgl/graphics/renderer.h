#pragma once

#include <glm/glm.hpp>
#include "mesh.h"
#include "material.h"

namespace fluxgl {
    struct RenderState {
        int framebufferWidth = 0;
        int framebufferHeight = 0;

        int viewportX = 0;
        int viewportY = 0;
        int viewportWidth = 0;
        int viewportHeight = 0;
        float targetAspectRatio = 16.0f / 9.0f;

        glm::vec3 clearColor = {0.1f, 0.1f, 0.1f};
    };

    struct AmbientLight {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
    };

    struct DirectionalLight {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        glm::vec3 direction = {0.0f, 0.0f, 1.0f};
    };

    struct PointLight {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
    };

    struct SceneState {
        std::vector<AmbientLight> ambientLights;
        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
    };

    class Renderer {
        private:
            Renderer() = default;
            ~Renderer() = default;
            static RenderState m_state;
            static SceneState m_sceneState;

        public:
            static void setFramebufferSize(int width, int height);
            static void beginFrame();

            static void setCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& position);
            static void setClearColor(const glm::vec3& color);
            static void registerAmbientLight(const glm::vec3& lightColor, const float intensity);
            static void registerDirectionalLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& direction);
            static void registerPointLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& position);

            static void drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix = glm::mat4(1.0f));
    };
}