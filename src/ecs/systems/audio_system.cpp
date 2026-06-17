#include <fluxgl/core/app.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/audio/audio_engine.h>

#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/systems/audio_system.h>
#include <fluxgl/ecs/components/audio_listener.h>
#include <fluxgl/ecs/components/audio_source.h>
#include <fluxgl/ecs/components/transform.h>

namespace fluxgl {
    void AudioSystem::onUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto& resources = scene.getContext().resourceManager;

        // Setup the listener (one per scene only, grab the first one)
        for(auto& e : registry.query<AudioListener, Transform>()) {
            auto& listener = e.getComponent<AudioListener>();
            auto& transform = e.getComponent<Transform>();

            if(listener.active) {
                AudioEngine::get().setListenerPosition(transform.position, transform.front(), transform.up());
                break;
            }
        }

        // Handle audio sources
        for(auto& e : registry.query<AudioSource>()) {
            auto& source = e.getComponent<AudioSource>();
            Sound *sound = resources.getSound(source.sound);

            // Update the source first
            if(!AudioEngine::get().isPlaying(sound)) {
                AudioEngine::get().stop(sound);
            }

            if(source.shouldPlay && sound->isValid()) {
                source.shouldPlay = false;
                AudioEngine::get().play(sound);
            }
            
            if(sound->isValid()) {
                if(source.shouldStop) {
                    source.shouldStop = false;
                    AudioEngine::get().stop(sound);
                }

                if(sound->getSpatialized() && e.hasComponent<Transform>()) {
                    auto& transform = e.getComponent<Transform>();
                    AudioEngine::get().setSoundPosition(sound, transform.position);
                }
            }
        }
    }
}