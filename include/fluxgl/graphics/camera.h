#pragma once

#include <fluxgl/core/transform.h>
#include <fluxgl/graphics/renderer.h>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>


namespace fluxgl {
    struct Camera {
        static constexpr const char* TypeName = "fluxgl::Camera";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            Camera camera;

            if(data.isObject()) {
                camera.fov = data.get("fov", 45.0f).asFloat();
                
                camera.nearPlane = data.get("nearPlane", 0.1f).asFloat();
                camera.farPlane = data.get("farPlane", 100.0f).asFloat();
                
                camera.isOrthographic = data.get("isOrthographic", false).asBool();
                camera.worldWidth = data.get("worldWidth", 24).asInt();
            }

            registry.addComponent<Camera>(entityID, camera);
        }

        float fov = 45.0f;

        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        bool isOrthographic = false;
        int worldWidth = 24;

        glm::vec2 getWorldDimensions() const {
            float aspectRatio = fluxgl::Renderer::getViewportAspectRatio();
            return {worldWidth, worldWidth / aspectRatio}; 
        }

        glm::mat4 getViewMatrix(Transform& transform) const {
            return glm::lookAt(transform.position, transform.position + transform.front(), transform.up());
        }

        glm::mat4 getProjectionMatrix() const {
            float aspectRatio = fluxgl::Renderer::getViewportAspectRatio();
            if (isOrthographic) {
                int worldHeight = worldWidth / aspectRatio;
                return glm::ortho(
                    -worldWidth * 0.5f, 
                    worldWidth * 0.5f, 
                    -worldHeight * 0.5f, 
                    worldHeight * 0.5f, 
                    nearPlane, 
                    farPlane
                );
            } else {
                return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
            }
        }
    };
}