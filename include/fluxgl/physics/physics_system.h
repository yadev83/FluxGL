#pragma once

#include <unordered_set>
#include <optional>

#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/system.h>
#include <fluxgl/physics/rigidbody.h>
#include <fluxgl/physics/box_collider.h>

namespace fluxgl {
    using CollisionPair = std::pair<EntityID, EntityID>;
    struct PairHash {
        size_t operator()(const CollisionPair& pair) const {
            return std::hash<EntityID>()(pair.first) ^ std::hash<EntityID>()(pair.second);
        }
    };

    enum class CollisionType {
        ENTER,
        STAY,
        EXIT
    };

    struct CollisionEvent {
        CollisionType type;
        Entity a;
        Entity b;

        glm::vec2 mtv;
    };

    struct RaycastHit2D {
        Entity entity;
        float distance = 0.0f;
        glm::vec3 point = glm::vec3(0.0f);
    };

    class PhysicsSystem : public System {
        private:
            std::vector<CollisionEvent> m_collisionEvents;
            float epsilon = 0.01f;
            glm::vec2 gravity = {0.0f, -9.81f};

            std::unordered_set<CollisionPair, PairHash> m_collisions;
            std::unordered_set<CollisionPair, PairHash> m_previousCollisions;

            bool canBeMoved(RigidBodyType type);
        public:
            std::vector<CollisionEvent>& getCollisions();
            void onFixedUpdate(Scene& scene, float dt) override;
            void onLateUpdate(Scene& scene, float dt) override;

            // Stateless methods that can be called manually
            static std::optional<RaycastHit2D> raycast(Registry& registry, const Ray& ray, bool ignoreTriggers = true);
            static std::vector<RaycastHit2D> raycastAll(Registry& registry, const Ray& ray, bool ignoreTriggers = true);
    };
}