#pragma once

#include <fluxgl/graphics/sprite.h>
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
            static glm::vec2 getFramebufferSize();
            static float getViewportAspectRatio();
            static glm::vec2 getViewportSize();
            static glm::vec2 getViewportPosition();
            static void beginFrame();
            static void beginUIPass();
            static void endUIPass();

            static glm::vec2 screenToViewport(glm::vec2 position);

            static void setCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& position);
            static void setClearColor(const glm::vec3& color);
            static void registerAmbientLight(const glm::vec3& lightColor, const float intensity);
            static void registerDirectionalLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& direction);
            static void registerPointLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& position);

            static void drawSprite(
                const glm::mat4& modelMatrix = glm::mat4(1.0f),

                const Shader* shader = nullptr,
                const Texture* texture = nullptr,
                const int layer = 0,
                const glm::vec3& color = glm::vec3(1.0f),
                const glm::vec2& size = glm::vec2(1.0f),
                const glm::vec2& uvMin = glm::vec2(0.0f),
                const glm::vec2& uvMax = glm::vec2(1.0f)
            );

            static void drawUIQuad(
                const glm::mat4& modelMatrix = glm::mat4(1.0f),
                
                const Shader* shader = nullptr,
                const Texture* texture = nullptr,
                const int layer = 0,
                const glm::vec4& color = glm::vec4(1.0f),
                const glm::vec2& size = glm::vec2(1.0f),
                const glm::vec2& uvMin = glm::vec2(0.0f),
                const glm::vec2& uvMax = glm::vec2(1.0f)
            );

            static void drawMesh(
                const Mesh* mesh = nullptr,
                const glm::mat4& modelMatrix = glm::mat4(1.0f),
                
                const Shader* shader = nullptr,
                const std::vector<Texture*> textures = {},
                const Texture* normalMap = nullptr,
                const Texture* specularMap = nullptr,
                const Texture* emissionMap = nullptr,

                const glm::vec3& albedoColor = glm::vec3(1.0f),
                const glm::vec3& specularColor = glm::vec3(1.0f),
                const glm::vec3& emissionColor = glm::vec3(0.0f),
                const float shininess = 32.0f
            );
    };
}