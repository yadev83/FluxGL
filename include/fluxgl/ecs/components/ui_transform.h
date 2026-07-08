#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    struct UITransform {
        static constexpr const char* TypeName = "fluxgl::UITransform";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            UITransform transform;
            
            // Parse the json data if it is a valid object, and go through its members to compute the transform component
            if(data.isObject()) {
                Json::Value positionData = data.get("position", Json::objectValue);
                transform.position = {positionData.get("x", 0.0f).asFloat(), positionData.get("y", 0.0f).asFloat()};

                Json::Value scaleData = data.get("scale", Json::objectValue);
                transform.scale = {scaleData.get("x", 1.0f).asFloat(), scaleData.get("y", 1.0f).asFloat()};

                transform.layer = data.get("layer", 0).asInt();
            }

            registry.addComponent<UITransform>(entityID, transform);
        }

        glm::vec2 position {0, 0};
        glm::vec2 scale = {1, 1};
        int layer = 0;
    };
}