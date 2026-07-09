#include <fluxgl/ecs/systems/audio_system.h>
#include <fluxgl/ecs/components/audio_source.h>

#include <fluxgl/ecs/entity.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/app.h>
#include <fluxgl/core/log.h>

#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/assets/resource_manager.h>

namespace fluxgl {
    void AudioSystem::onUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto& audioEngine = scene.getContext().audioEngine;
        auto& resourceManager = scene.getContext().resourceManager;

        // Ask the engine to cleanup unused voices first
        audioEngine.cleanupUnusedVoices();

        for(auto e : registry.query<AudioSource>()) {
            if(!e.isEnabled()) continue;
            auto& source = e.getComponent<AudioSource>();

            // CREATE
            if(source.shouldPlay && !audioEngine.isValid(source.handle)) {
                auto sound = resourceManager.getResource<Sound>(source.sound);
                source.handle = audioEngine.play(
                    sound,
                    { source.volume, source.looping }
                );

                source.shouldPlay = false;
                continue;
            }

            // PAUSE / RESUME / STOP
            if (audioEngine.isValid(source.handle)) {
                bool enginePlaying = audioEngine.isPlaying(source.handle);
                if (source.shouldPause && enginePlaying) {
                    audioEngine.pause(source.handle);
                } else if (source.shouldPlay && !enginePlaying) {
                    audioEngine.resume(source.handle);
                } 
                
                if (source.shouldStop) {
                    audioEngine.stop(source.handle);
                }

                source.shouldPause = false;
                source.shouldPlay = false;
                source.shouldStop = false;
            }
        }
    }
}