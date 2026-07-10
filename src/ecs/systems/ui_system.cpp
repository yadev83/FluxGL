#include <fluxgl/ecs/systems/ui_system.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/app.h>
#include <fluxgl/core/log.h>

#include <fluxgl/graphics/renderer.h>
#include <fluxgl/ecs/components/ui_transform.h>
#include <fluxgl/ecs/components/ui_collider.h>

namespace fluxgl {
    void UISystem::onUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto& inputManager = scene.getContext().inputManager;

        glm::vec2 mousePos = {inputManager.getMouseX(), inputManager.getMouseY()};

        for(auto e : registry.query<UICollider, UITransform>()) {
            if(!e.isEnabled()) continue;
            
            auto& collider = e.getComponent<UICollider>();
            auto& transform = e.getComponent<UITransform>();

            auto colliderAABB = collider.getAABB(transform, Renderer::getFramebufferSize());
            if(colliderAABB.contains(mousePos)) {
                FLUXGL_LOG_TRACE("UI SYSTEM : MOUSE IN COLLIDER " + e.getID());
            }
        }
    }
}