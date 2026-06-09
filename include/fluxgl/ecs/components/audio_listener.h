#pragma once

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    struct AudioListener {
        static constexpr const char* TypeName = "fluxgl::AudioListener";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            AudioListener listener;

            if(data.isObject()) {
                listener.active = data.get("active", true).asBool();
            }

            registry.addComponent<AudioListener>(entityID, listener);
        }

        bool active = true;
    };
}