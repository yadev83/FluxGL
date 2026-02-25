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

        // Update the audio engine
        AudioEngine::get().update();

        // Setup the listener (one per scene only, grab the first one)
        for(auto& e : registry.query<AudioListener, Transform>()) {
            auto& listener = e.getComponent<AudioListener>();
            auto& transform = e.getComponent<Transform>();

            if(listener.active) {
                AudioEngine::get().setListener(transform.position, transform.front(), transform.up());
                break;
            }
        }

        // Handle audio sources
        for(auto& e : registry.query<AudioSource>()) {
            auto& source = e.getComponent<AudioSource>();

            if(source.shouldPlay && source.source == 0) {
                source.shouldPlay = false;
                SoundID sound = AudioEngine::get().loadSound(source.sound.path, source.sound.type);
                source.source = AudioEngine::get().play(sound, source.loop, source.volume);
            }
            
            if(source.source != 0) {
                if(!AudioEngine::get().isPlaying(source.source)) {
                    source.source = 0;
                }

                if(source.shouldStop) {
                    source.shouldStop = false;
                    AudioEngine::get().stop(source.source);
                }

                if(source.spatialized && e.hasComponent<Transform>()) {
                    auto& transform = e.getComponent<Transform>();
                    AudioEngine::get().setSourcePosition(source.source, transform.position);
                    AudioEngine::get().setSourceSpatialized(source.source, true);
                } else {
                    AudioEngine::get().setSourceSpatialized(source.source, false);
                }
            }
        }
    }
}