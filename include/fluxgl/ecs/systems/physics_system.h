#pragma once

#include <unordered_set>

#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/system.h>
#include <fluxgl/ecs/components/rigidbody.h>
#include <fluxgl/ecs/components/box_collider.h>

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
    };
}