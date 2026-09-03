#pragma once

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    struct Lifetime {
        static constexpr const char* TypeName = "fluxgl::Lifetime";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            float seconds = data.get("seconds", 0.0f).asFloat();
            Lifetime lifetime(seconds);
            registry.addComponent<Lifetime>(entityID, lifetime);
        }

        float remaining = 0.0f;

        Lifetime(float seconds = 0.0f) : remaining(seconds) {}
    };
}