#pragma once

#include <fluxgl/math/ray.h>
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

        Ray screenPointToRay(Transform& transform, float x, float y) const {
            glm::vec2 viewportPosition = Renderer::getViewportPosition();
            glm::vec2 viewportSize = Renderer::getViewportSize();
            glm::vec2 framebufferSize = Renderer::getFramebufferSize();

            // glm::unProject attend des coordonnées "fenêtre" OpenGL (origine en bas à gauche),
            // alors que x,y arrivent généralement en coordonnées écran (origine en haut à gauche).
            // On inverse donc l'axe Y sur la hauteur du framebuffer, puis on fournit à unProject
            // la position réelle du viewport afin de gérer les letterbox.
            glm::vec4 viewport(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);
            float winX = x;
            float winY = framebufferSize.y - y;

            glm::mat4 view = getViewMatrix(transform);
            glm::mat4 proj = getProjectionMatrix();

            // Un point sur le plan near (z=0) et un sur le plan far (z=1) du même pixel
            glm::vec3 nearPoint = glm::unProject(glm::vec3(winX, winY, 0.0f), view, proj, viewport);
            glm::vec3 farPoint  = glm::unProject(glm::vec3(winX, winY, 1.0f), view, proj, viewport);

            if (isOrthographic) {
                // En ortho, tous les rayons sont parallèles : seule l'origine change par pixel,
                // la direction est celle du regard de la caméra.
                return Ray{ nearPoint, glm::normalize(transform.front()) };
            } else {
                // En perspective, tous les rayons partent de l'œil et divergent vers l'écran.
                return Ray{ nearPoint, glm::normalize(farPoint - nearPoint) };
            }
        }

        // Projette le point écran sur une plan XY monde (worldZ = 0 par défaut).
        // Confort pour le 2D : c'est le plan où vivent les entités (sprites, colliders, ...).
        // Pour la 3D, préférez screenToWorldPointOnPlane qui ne suppose pas un plan XY.
        glm::vec3 screenToWorldPoint(Transform& transform, float screenX, float screenY, float worldZ = 0.0f) const {
            Ray ray = screenPointToRay(transform, screenX, screenY);

            if (std::abs(ray.direction.z) < 1e-8f) {
                return glm::vec3(ray.origin.x, ray.origin.y, worldZ);
            }

            float t = (worldZ - ray.origin.z) / ray.direction.z;
            return ray.origin + ray.direction * t;
        }

        // Projette le point écran sur un plan monde arbitraire (point + normale).
        // Générique pour la 3D : sol, mur, plan d'une entité cible, etc.
        glm::vec3 screenToWorldPointOnPlane(Transform& transform, float screenX, float screenY, const glm::vec3& planePoint, const glm::vec3& planeNormal) const {
            Ray ray = screenPointToRay(transform, screenX, screenY);

            float denom = glm::dot(ray.direction, planeNormal);
            if (std::abs(denom) < 1e-8f) return ray.origin;

            float t = glm::dot(planePoint - ray.origin, planeNormal) / denom;
            if (t < 0.0f) return ray.origin;

            return ray.origin + ray.direction * t;
        }
    };
}