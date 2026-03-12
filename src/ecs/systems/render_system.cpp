#include <vector>
#include <fluxgl/core/scene.h>

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/systems/render_system.h>

#include <fluxgl/ecs/components/transform.h>
#include <fluxgl/ecs/components/camera.h>
#include <fluxgl/ecs/components/mesh_renderer.h>
#include <fluxgl/ecs/components/light.h>

#include <fluxgl/graphics/renderer.h>
#include <fluxgl/graphics/debug_renderer.h>

namespace fluxgl {
    void RenderSystem::onUpdate(fluxgl::Scene& scene, float dt) {
        auto& registry = scene.getRegistry();

        // Clear/Setup frame
        Renderer::beginFrame();

        // Setup lights
        for(auto entity : registry.query<Light>()) {
            auto& light = entity.getComponent<Light>();

            switch(light.type) {
                case LightType::Directional: {
                    if(entity.hasComponent<Transform>()) {
                        auto& transform = entity.getComponent<Transform>();
                        Renderer::registerDirectionalLight(light.color, light.intensity, transform.front());
                    } 
                } break;
                case LightType::Point: {
                    if(entity.hasComponent<Transform>()) {
                        auto& transform = entity.getComponent<Transform>();
                        Renderer::registerPointLight(light.color, light.intensity, transform.position);
                    }
                } break;
                case LightType::Ambient:
                    Renderer::registerAmbientLight(light.color, light.intensity);
                    break;
            }
        }

        // Find the active camera
        Camera* camera = nullptr;
        Transform* cameraTransform = nullptr;
        
        for(auto entity : registry.query<Camera, Transform>()) {
            camera = &entity.getComponent<Camera>();
            cameraTransform = &entity.getComponent<Transform>();
            break;
        }

        // Set the camera
        if(camera && cameraTransform) {
            glm::mat4 viewMatrix = camera->getViewMatrix(*cameraTransform);
            glm::mat4 projectionMatrix = camera->getProjectionMatrix();
            Renderer::setCamera(viewMatrix, projectionMatrix, cameraTransform->position);
            if(DebugRenderer::isEnabled()) DebugRenderer::setCamera(viewMatrix, projectionMatrix);
        }

        // Draw sprites
        for(auto entity : registry.query<Sprite, Transform>()) {
            auto& sprite = entity.getComponent<Sprite>();
            auto& transform = entity.getComponent<Transform>();

            static Mesh quad = Mesh::quad();
            Renderer::drawSprite(quad, sprite, transform.getModelMatrix());
        }

        // Draw meshes
        for(auto entity : registry.query<MeshRenderer, Transform>()) {
            auto& meshRenderer = entity.getComponent<MeshRenderer>();
            auto& transform = entity.getComponent<Transform>();
            
            Renderer::drawMesh(
                meshRenderer.mesh, 
                meshRenderer.material, 
                transform.getModelMatrix()
            );
        }

        // Debug renderer (if enabled)
        if(DebugRenderer::isEnabled()) DebugRenderer::flush();
    }
}