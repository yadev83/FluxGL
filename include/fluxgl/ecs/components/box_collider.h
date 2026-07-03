#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/components/transform.h>

namespace fluxgl {
    enum class CollisionLayer {
        Default,    // Default layer : everything collides with everything
        Hitbox,     // Hitbox layer : hitboxes are usually triggers that collide with hurtboxes
        Hurtbox     // Hurtbox layer : collides with hitboxes
    };

    struct AABB {
        glm::vec2 min = {0.0f, 0.0f};
        glm::vec2 max = {0.0f, 0.0f};

        bool intersects(const AABB& other) const {
            return
                min.x <= other.max.x &&
                max.x >= other.min.x &&
                min.y <= other.max.y &&
                max.y >= other.min.y;
        }
    };

    struct BoxCollider2D {
        static constexpr const char* TypeName = "fluxgl::BoxCollider2D";
        static void Create(Registry& registry, EntityID entityID, const Json::Value& data) {
            BoxCollider2D boxCollider2D;

            if(data.isObject()) {
                Json::Value layerData = data.get("layer", "default");
                if(layerData.asString() == "default") boxCollider2D.layer = CollisionLayer::Default;
                if(layerData.asString() == "hitbox") boxCollider2D.layer = CollisionLayer::Hitbox;
                if(layerData.asString() == "hurtbox") boxCollider2D.layer = CollisionLayer::Hurtbox;

                boxCollider2D.isTrigger = data.get("isTrigger", false).asBool();

                Json::Value halfSizeData = data.get("halfSize", Json::Value());
                if(halfSizeData.isObject()) {
                    boxCollider2D.halfSize.x = halfSizeData.get("x", 0.5f).asFloat();
                    boxCollider2D.halfSize.y = halfSizeData.get("y", 0.5f).asFloat();
                }
            }

            registry.addComponent<BoxCollider2D>(entityID, boxCollider2D);
        }
        
        glm::vec2 halfSize = {0.5f, 0.5f};
        glm::vec2 offset = {0.0f, 0.0f}; 

        CollisionLayer layer = CollisionLayer::Default;
        bool isTrigger = false;

        AABB getAABB(const Transform& transform) const {
            AABB aabb;

            glm::vec2 center2D = glm::vec2(transform.position) + offset;
            aabb.min = center2D - halfSize;
            aabb.max = center2D + halfSize;

            return aabb;
        }

        bool shouldCollide(const BoxCollider2D& other) const { 
            if(layer == CollisionLayer::Hitbox && other.layer == CollisionLayer::Hurtbox) return true;
            if(layer == CollisionLayer::Hurtbox && other.layer == CollisionLayer::Hitbox) return true;

            if(layer == CollisionLayer::Default || other.layer == CollisionLayer::Default) return true;

            return false;
        }
    };
}