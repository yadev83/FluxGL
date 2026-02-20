#include <vector>
#include <fluxgl/core/scene.h>

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/systems/render_system.h>

#include <fluxgl/ecs/components/transform.h>
#include <fluxgl/ecs/components/camera.h>
#include <fluxgl/ecs/components/mesh_renderer.h>

#include <fluxgl/graphics/renderer.h>

namespace fluxgl {
    void RenderSystem::onUpdate(fluxgl::Scene& scene, float dt) {
        auto& registry = scene.getRegistry();

        Renderer::clear({0.1f, 0.1f, 0.1f});

        // Find the active camera
        Camera* camera = nullptr;
        Transform* cameraTransform = nullptr;
        
        for(auto entity : registry.query<Camera, Transform>()) {
            camera = &entity.getComponent<Camera>();
            cameraTransform = &entity.getComponent<Transform>();
            break;
        }

        for(auto entity : registry.query<MeshRenderer, Transform>()) {
            auto& meshRenderer = entity.getComponent<MeshRenderer>();
            auto& transform = entity.getComponent<Transform>();
            
            Renderer::drawMesh(
                meshRenderer.mesh, 
                meshRenderer.material, 
                transform.getModelMatrix(), 
                camera && cameraTransform ? camera->getViewMatrix(*cameraTransform) : glm::mat4(1.0f), 
                camera ? camera->getProjectionMatrix() : glm::mat4(1.0f)
            );
        }
    }
}