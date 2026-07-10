#pragma once

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

#include <fluxgl/ecs/components/ui_transform.h>
#include <fluxgl/math/aabb.h>

#include <json/json.h>
#include <glm/glm.hpp>

namespace fluxgl {
    struct UICollider {
        static constexpr const char* TypeName = "fluxgl::UICollider";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            UICollider collider;
            
            // Parse the json data if it is a valid object, and go through its members to compute the collider component
            if(data.isObject()) {
                Json::Value sizeData = data.get("size", Json::objectValue);
                collider.size = {sizeData.get("x", 1.0).asFloat(), sizeData.get("y", 1.0).asFloat()};
            }

            registry.addComponent<UICollider>(entityID, collider);
        }

        glm::vec2 size;

        inline AABB getAABB(const UITransform& transform, const glm::vec2& screenSize) const {
            glm::vec2 transformPosition = glm::vec2(transform.getComputedPosition(size, screenSize));
            
            // Compute top left (for min) agnostic of pivot used by the UITransform (rempove the pivot offset)
            glm::vec2 min = transformPosition - transform.getPivotOffset(size);
            glm::vec2 max = min + size;

            return {min, max};
        } 
    };
}