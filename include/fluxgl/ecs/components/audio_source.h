#pragma once

#include <string>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>

#include <fluxgl/audio/source.h>
#include <fluxgl/audio/sound.h>

namespace fluxgl {
    using SourceID = unsigned int;

    struct AudioSource {
        static constexpr const char* TypeName = "fluxgl::AudioSource";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            AudioSource source;

            registry.addComponent<AudioSource>(entityID, source);
        }

        Resource sound;

        bool shouldPlay = false;
        bool shouldStop = false;
    };
}