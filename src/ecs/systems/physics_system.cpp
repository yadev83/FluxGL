#include <fluxgl/ecs/systems/physics_system.h>
#include <fluxgl/graphics/debug_renderer.h>
#include <fluxgl/ecs/components/facing.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    bool PhysicsSystem::canBeMoved(RigidBodyType type) {
        return type == RigidBodyType::Dynamic;
    }

    std::vector<CollisionEvent>& PhysicsSystem::getCollisions() { 
        return m_collisionEvents; 
    }

    void PhysicsSystem::onFixedUpdate(Scene& scene, float dt) {
        // Grab registry
        auto& registry = scene.getRegistry();

        // Clean up old collisions
        m_collisionEvents.clear();
        m_previousCollisions = m_collisions;
        m_collisions.clear();

        // Gravity & Integration
        for(auto e : registry.query<RigidBody2D, Transform>()) {
            if(!e.isEnabled()) continue;
            auto& rigidbody = e.getComponent<RigidBody2D>();
            auto& transform = e.getComponent<Transform>();

            // No physics integration for static objects
            if(rigidbody.type == RigidBodyType::Static) continue;

            // Apply forces only on dynamic objects
            if(rigidbody.type == RigidBodyType::Dynamic) {
                if(!rigidbody.grounded) rigidbody.velocity += gravity * rigidbody.gravityScale * dt;
            }

            // Integration is for both dynamic and kinematic
            transform.position += glm::vec3(rigidbody.velocity, 0.0f) * dt;
            rigidbody.grounded = false;
        }

        // Collision detection and resolution
        auto collidables = registry.query<BoxCollider2D, Transform>();

        for(auto a : collidables) {
            for(auto b : collidables) {
                if(!(a.isEnabled() && b.isEnabled())) continue;

                // Skip self collision check and avoid doubles
                if(a.getID() >= b.getID()) continue;
                // Make sure that both a and b are valid entities
                if(!a.isValid() || !b.isValid()) continue;

                // Skip check if entities hold each other or are owned by the same
                if(a.getParent().isValid() && a.getParent().getID() == b.getID()) continue; // a is b's child
                if(b.getParent().isValid() && b.getParent().getID() == a.getID()) continue; // b is a's child
                if(a.getParent().isValid() && b.getParent().isValid() && a.getParent().getID() == b.getParent().getID()) continue; // a and b are owned by the same entity

                auto& ca = a.getComponent<BoxCollider2D>();
                auto& ta = a.getComponent<Transform>();
                EntityID aID = a.getID();
                auto aabbA = ca.getAABB(ta);

                auto& cb = b.getComponent<BoxCollider2D>();
                auto& tb = b.getComponent<Transform>();
                EntityID bID = b.getID();
                auto aabbB = cb.getAABB(tb);

                // Start checking if collision should happen
                bool shouldCollide = ca.shouldCollide(cb) && aabbA.intersects(aabbB);
                if(!shouldCollide) continue;

                /* From this point, there is a collision */
                // Axis-Aligned resolution from here
                float overlapTop    = aabbB.max.y - aabbA.min.y;  // A falls on B
                float overlapBottom = aabbB.min.y - aabbA.max.y; // A hits B from under
                float overlapLeft  = aabbB.max.x - aabbA.min.x;  // A on the left
                float overlapRight = aabbB.min.x - aabbA.max.x;  // A on the right
                float mtvX = (std::abs(overlapLeft) < std::abs(overlapRight)) ? overlapLeft : overlapRight;
                float mtvY = (std::abs(overlapTop) < std::abs(overlapBottom)) ? overlapTop : overlapBottom;

                // Store collision pair
                CollisionPair collisionPair = {
                    (((aID) < (bID)) ? (aID) : (bID)), 
                    (((aID) > (bID)) ? (aID) : (bID))
                };
                if (m_collisions.contains(collisionPair)) continue; // skip if collision pair already exists

                m_collisions.insert(collisionPair); // min/max to avoid duplicates

                // Check if collision is a stay or an enter
                if(m_previousCollisions.contains(collisionPair)) {
                    m_collisionEvents.push_back({
                        CollisionType::STAY,
                        a,
                        b,
                        {mtvX, mtvY}
                    });
                } else {
                    m_collisionEvents.push_back({
                        CollisionType::ENTER,
                        a,
                        b,
                        {mtvX, mtvY}
                    });
                }

                if(ca.isTrigger || cb.isTrigger) continue; // If one of the colliders is a trigger, skip the physical resolution
                if(!(a.hasComponent<RigidBody2D>() && b.hasComponent<RigidBody2D>())) continue; // If neither has a rigidbody, skip as well

                // Grab rigidbody components and check if they can move
                auto& rba = a.getComponent<RigidBody2D>();
                auto& rbb = b.getComponent<RigidBody2D>();
                if(!canBeMoved(rba.type) && !canBeMoved(rbb.type)) continue;

                // Only correct on the smallest penetration axis
                if(std::abs(mtvX) < std::abs(mtvY)) {
                    if(canBeMoved(rba.type)) {
                        ta.position.x += canBeMoved(rbb.type) ? (mtvX * 0.5f) : mtvX;
                        if(mtvX != 0.0f) rba.velocity.x = 0.0f;
                    }

                    if(canBeMoved(rbb.type)) {
                        tb.position.x -= canBeMoved(rba.type) ? (mtvX * 0.5f) : mtvX;
                        if(mtvX != 0.0f) rbb.velocity.x = 0.0f;
                    }
                } else {
                    if(canBeMoved(rba.type)) {
                        ta.position.y += canBeMoved(rbb.type) ? (mtvY * 0.5f) : mtvY;
                        if(mtvY > 0.0f && rba.velocity.y <= 0.0f) rba.grounded = true;
                        if(mtvY != 0.0f) rba.velocity.y = 0.0f;
                    }

                    if(canBeMoved(rbb.type)) {
                        tb.position.y -= canBeMoved(rba.type) ? (mtvY * 0.5f) : mtvY;
                        if(mtvY < 0.0f && rbb.velocity.y <= 0.0f) rbb.grounded = true;
                        if(mtvY != 0.0f) rbb.velocity.y = 0.0f;
                    }
                }
            }
        }

        // Emit exit collision events
        for(auto& pair : m_previousCollisions) {
            if(!m_collisions.contains(pair)) {
                auto a = registry.getEntity(pair.first);
                auto b = registry.getEntity(pair.second);

                m_collisionEvents.push_back({
                    CollisionType::EXIT,
                    a,
                    b,
                    {0.0f, 0.0f}
                });
            }
        }
    }

    void PhysicsSystem::onLateUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto collidables = registry.query<BoxCollider2D, Transform>();

        // Render debug lines
        for(auto entity : collidables) {
            // Check if the entity is in one collision
            bool inCollision = false;
            for(auto collision : m_collisionEvents) {
                if(collision.a.getID() == entity.getID() || collision.b.getID() == entity.getID()) {
                    inCollision = true;
                    break;
                }
            }

            // Box color
            glm::vec3 color = inCollision ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);

            // Render the box
            auto& transform = entity.getComponent<Transform>();
            auto& collider = entity.getComponent<BoxCollider2D>();
            auto aabb = collider.getAABB(transform);
            DebugRenderer::rect(aabb.min, aabb.max, color);
            DebugRenderer::cross({transform.position.x, transform.position.y}, glm::vec3(1.0f));

            // Check if the entity has a facing direction to show
            if(entity.hasComponent<Facing2D>()) {
                auto& facing = entity.getComponent<Facing2D>();
                glm::vec3 facingColor = glm::vec3(1.0f, 1.0f, 0.0f);
                float facingLength = (facing.direction == FacingDirection::Right) ? 1.0f : -1.0f;
                
                DebugRenderer::line(transform.position, transform.position + transform.worldRight() * facingLength, facingColor);
            }
        }
    }
}