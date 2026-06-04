#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    enum class LightType { Ambient, Directional, Point };

    struct Light {
        static constexpr const char* TypeName = "Light";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            Light light;
            registry.addComponent<Light>(entityID, light);
        }

        LightType type = LightType::Ambient;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
    };
}