#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>

namespace fluxgl {
    struct UIRect {
        static constexpr const char* TypeName = "fluxgl::UIRect";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            UIRect rect;
            
            // Parse the json data if it is a valid object, and go through its members to compute the rect component
            if(data.isObject()) {
                rect.texture = data.get("texture", "").asString();
                rect.shader = data.get("shader", "").asString();
                
                Json::Value colorData = data.get("color", Json::objectValue);
                rect.color = {colorData.get("r", 1.0f).asFloat(), colorData.get("g", 1.0f).asFloat(), colorData.get("b", 1.0f).asFloat(), colorData.get("a", 1.0f).asFloat()};

                Json::Value uvMinData = data.get("uvMin", Json::objectValue);
                rect.uvMin = {uvMinData.get("x", 0.0f).asFloat(), uvMinData.get("y", 0.0f).asFloat()};

                Json::Value uvMaxData = data.get("uvMax", Json::objectValue);
                rect.uvMax = {uvMaxData.get("x", 1.0f).asFloat(), uvMaxData.get("y", 1.0f).asFloat()};
            }

            registry.addComponent<UIRect>(entityID, rect);
        }
        
        Resource texture;
        Resource shader;

        glm::vec4 color = {1, 1, 1, 1};

        glm::vec2 uvMin = {0, 0};
        glm::vec2 uvMax = {1, 1};
    };
}