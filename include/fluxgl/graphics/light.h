#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    enum class LightType { Ambient, Directional, Point };

    struct Light {
        static constexpr const char* TypeName = "fluxgl::Light";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            Light light;

            if(data.isObject()) {
                Json::Value typeData = data.get("type", "ambient");
                if(typeData.asString() == "ambient") light.type = LightType::Ambient;
                if(typeData.asString() == "directional") light.type = LightType::Directional;
                if(typeData.asString() == "point") light.type == LightType::Point;

                Json::Value colorData = data.get("color", Json::objectValue);
                light.color = {colorData.get("r", 1.0f).asFloat(), colorData.get("g", 1.0f).asFloat(), colorData.get("b", 1.0f).asFloat()};
                
                light.intensity = data.get("intensity", 1.0f).asFloat();
            }

            registry.addComponent<Light>(entityID, light);
        }

        LightType type = LightType::Ambient;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
    };
}