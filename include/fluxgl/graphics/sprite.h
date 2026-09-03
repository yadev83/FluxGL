#pragma once

#include <json/json.h>
#include <glm/glm.hpp>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>

namespace fluxgl {
    struct Sprite {
        static constexpr const char* TypeName = "fluxgl::Sprite";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            Sprite sprite;

            if(data.isObject()) {
                Json::Value colorData = data.get("color", Json::objectValue);
                sprite.color = {colorData.get("r", 1.0f).asFloat(), colorData.get("g", 0.0f).asFloat(), colorData.get("b", 1.0f).asFloat()};

                Json::Value sizeData = data.get("size", Json::objectValue);
                sprite.size = {sizeData.get("x", 1.0f).asFloat(), sizeData.get("y", 1.0f).asFloat()};

                sprite.layer = data.get("layer", 0).asInt();
                
                Json::Value uvMinData = data.get("uvMin", Json::objectValue);
                sprite.uvMin = {uvMinData.get("x", 0.0f).asFloat(), uvMinData.get("y", 0.0f).asFloat()};

                Json::Value uvMaxData = data.get("uvMax", Json::objectValue);
                sprite.uvMax = {uvMaxData.get("x", 1.0f).asFloat(), uvMaxData.get("y", 1.0f).asFloat()};

                sprite.texture = data.get("texture", "").asString();
                sprite.shader = data.get("shader", "").asString();
            }

            registry.addComponent<Sprite>(entityID, sprite);
        }

        Resource texture;
        Resource shader;

        glm::vec3 color = {1.0f, 0.0f, 1.0f};
        glm::vec2 size = {1.0f, 1.0f};

        int layer = 0;
        glm::vec2 uvMin = {0.0f, 0.0f};
        glm::vec2 uvMax = {1.0f, 1.0f};
    };
}