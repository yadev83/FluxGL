#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/core/transform.h>
#include <fluxgl/math/aabb.h>

namespace fluxgl {
    enum class CollisionLayer {
        Default,    // Default layer : everything collides with everything
        Hitbox,     // Hitbox layer : hitboxes are usually triggers that collide with hurtboxes
        Hurtbox     // Hurtbox layer : collides with hitboxes
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

                Json::Value sizeData = data.get("size", Json::Value());
                if(sizeData.isObject()) {
                    boxCollider2D.size.x = sizeData.get("x", 1.0f).asFloat();
                    boxCollider2D.size.y = sizeData.get("y", 1.0f).asFloat();
                }
            }

            registry.addComponent<BoxCollider2D>(entityID, boxCollider2D);
        }
        
        glm::vec2 size = {1.0f, 1.0f};
        glm::vec2 offset = {0.0f, 0.0f}; 

        CollisionLayer layer = CollisionLayer::Default;
        bool isTrigger = false;

        AABB getAABB(const Transform& transform) const {
            AABB aabb;

            glm::vec2 center2D = glm::vec2(transform.position) + offset;
            aabb.min = center2D - size * 0.5f;
            aabb.max = center2D + size * 0.5f;

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