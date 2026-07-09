#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>

namespace fluxgl {
    struct UIText {
        static constexpr const char* TypeName = "fluxgl::UIText";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            UIText text;
            
            // Parse the json data if it is a valid object, and go through its members to compute the text component
            if(data.isObject()) {
                text.text = data.get("text", "").asString();
                text.font = data.get("font", "").asString();
                text.shader = data.get("shader", "").asString();
                text.fontSize = data.get("fontSize", 32.0f).asFloat();
                text.maxWidth = data.get("maxWidth", 0.0f).asFloat();
                text.autoScale = data.get("autoScale", false).asBool();

                Json::Value colorData = data.get("color", Json::objectValue);
                text.color = {colorData.get("r", 1.0f).asFloat(), colorData.get("g", 1.0f).asFloat(), colorData.get("b", 1.0f).asFloat(), colorData.get("a", 1.0f).asFloat()};
            }

            registry.addComponent<UIText>(entityID, text);
        }

        std::string text;
        Resource font;
        Resource shader; 

        glm::vec4 color = {1, 1, 1, 1};

        float fontSize = 32.0f;

        float maxWidth = 0.0f;
        bool autoScale = false;
    };
}