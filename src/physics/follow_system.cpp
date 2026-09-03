#include <fluxgl/physics/follow_system.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/log.h>

#include <fluxgl/core/transform.h>
#include <fluxgl/physics/follow.h>

namespace fluxgl {
    void Follow2DSystem::onUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();

        for(auto e : registry.query<Transform, Follow2D>()) {
            auto& followComponent = e.getComponent<Follow2D>();
            
            EntityID targetID = followComponent.targetID;
            if(!registry.hasComponent<Transform>(targetID)) {
                FLUXGL_LOG_ERROR("Follow2DSystem::Can not follow target entity without a proper transform component");
                continue;
            }

            auto& targetTransform = registry.getComponent<Transform>(targetID);
            auto& transform = e.getComponent<Transform>();

            // Instant folllow if smoothing is disabled
            if(followComponent.smoothing <= 0.0f) {
                transform.position.x = targetTransform.position.x + followComponent.offset.x;
                transform.position.y = targetTransform.position.y + followComponent.offset.y;
                continue;
            }

            glm::vec2 targetPos = {targetTransform.position.x + followComponent.offset.x, targetTransform.position.y + followComponent.offset.y};
            glm::vec2 selfPos = {transform.position.x, transform.position.y};

            // Look ahead computation
            glm::vec2 velocity = (targetPos - followComponent.m_previousTargetPos) / dt;
            glm::vec2 desiredLookAhead = glm::vec2(
                (velocity.x > 0 ? 1.0f : velocity.x < 0 ? -1.0f : 0.0f) * followComponent.lookAheadDistance,
                0.0f
            );

            followComponent.m_lookAhead = glm::mix(followComponent.m_lookAhead, desiredLookAhead, followComponent.lookAheadSmoothing * dt);
            followComponent.m_previousTargetPos = targetPos;

            // Deadzone desired position computation
            glm::vec2 desired = selfPos;
            glm::vec2 delta = targetPos - selfPos;

            if(delta.x > followComponent.deadzone.x) desired.x = targetPos.x - followComponent.deadzone.x;
            if(delta.x < -followComponent.deadzone.x) desired.x = targetPos.x + followComponent.deadzone.x;
            if(delta.y > followComponent.deadzone.y) desired.y = targetPos.y - followComponent.deadzone.y;
            if(delta.y < -followComponent.deadzone.y) desired.y = targetPos.y + followComponent.deadzone.y;
            
            // Apply look ahead
            desired += followComponent.m_lookAhead;

            // Apply smoothing
            glm::vec2 smoothed = glm::mix(selfPos, desired, followComponent.smoothing * dt);

            // Set position
            transform.position.x = smoothed.x;
            transform.position.y = smoothed.y;
        }
    }
}