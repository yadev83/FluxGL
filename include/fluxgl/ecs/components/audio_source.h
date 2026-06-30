#pragma once

#include <json/json.h>

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>

#include <fluxgl/audio/audio_engine.h>

namespace fluxgl {
    struct AudioSource {
        static constexpr const char* TypeName = "fluxgl::AudioSource";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            AudioSource audioSource;

            if(data.isObject()) {
                audioSource.sound = data.get("sound", "").asString();
                audioSource.shouldPlay = data.get("shouldPlay", false).asBool();
                audioSource.looping = data.get("looping", false).asBool();
                audioSource.volume = data.get("volume", 1.0f).asFloat();
            }

            registry.addComponent<AudioSource>(entityID, audioSource);
        }

        Resource sound;
        AudioVoiceHandle handle = 0;

        bool shouldPlay = false;
        bool shouldPause = false;
        bool shouldStop = false;

        bool looping = false;
        float volume = 1.0f;
    };
}