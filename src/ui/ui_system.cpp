#include <fluxgl/ui/ui_system.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/app.h>
#include <fluxgl/core/log.h>

#include <fluxgl/graphics/renderer.h>
#include <fluxgl/ui/ui_transform.h>
#include <fluxgl/ui/ui_collider.h>
#include <fluxgl/ui/ui_button.h>

namespace fluxgl {
    void UISystem::onUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto& inputManager = scene.getContext().inputManager;

        // We have to compute the mouse position with respect to the viewport size (because scaling could have occured)
        glm::vec2 mousePos = Renderer::screenToViewport({inputManager.getMouseX(), inputManager.getMouseY()});

        for(auto e : registry.query<UICollider, UITransform>()) {
            if(!e.isEnabled()) continue;
            
            auto& collider = e.getComponent<UICollider>();
            auto& transform = e.getComponent<UITransform>();
            auto colliderAABB = collider.getAABB(transform, Renderer::getViewportSize());
            
            if(e.hasComponent<UIButton>()) {
                auto& button = e.getComponent<UIButton>();
                bool hovered = colliderAABB.contains(mousePos);

                button.hovered = hovered;
                button.pressed = hovered && inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
            }                
        }
    }
}