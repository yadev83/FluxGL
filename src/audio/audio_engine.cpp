#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/audio/sound.h>
#include <fluxgl/audio/source.h>

#include <fluxgl/core/log.h>

namespace fluxgl {
    AudioEngine& AudioEngine::get() {
        static AudioEngine instance;
        return instance;
    }

    ma_engine *AudioEngine::getEngine() {
        return &(get().m_engine);
    }

    bool AudioEngine::init() {
        if(ma_engine_init(NULL, &m_engine) == MA_SUCCESS) {
            FLUXGL_LOG_INFO("Audio Engine initialized");
            return true;
        } else {
            FLUXGL_LOG_ERROR("Failed to initialize audio engine");
            return false;
        }
    }

    void AudioEngine::shutdown() {
        FLUXGL_LOG_INFO("Shutting down Audio Engine");
        for(auto& [id, source]: m_sources) {
            ma_sound_uninit(&source.sound);
        }

        ma_engine_uninit(&m_engine);
    }

    // void AudioEngine::update() {
    //     // Clear old sources
    //     for(auto it = m_sources.begin(); it != m_sources.end();) {
    //         if(!isPlaying(it->first)) {
    //             stop(it->first);
    //         } else {
    //             ++it;
    //         }
    //     }

    //     // Mix volumes
    //     for(auto& [id, source] : m_sources) {
    //         auto it = m_sounds.find(id);
    //         if(it == m_sounds.end()) continue;
            
    //         float volume = source.volume * getVolume(it->second.type) * getMasterVolume();
    //         ma_sound_set_volume(&source.sound, volume);
    //     }
    // }

    void AudioEngine::setSoundPosition(Sound* sound, const glm::vec3& position) {
        if(!sound) return;
        ma_sound_set_position(&(sound->getSound()), position.x, position.y, position.z);
    }

    void AudioEngine::setListenerPosition(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {
        ma_engine_listener_set_position(
            &m_engine,
            0,
            position.x,
            position.y,
            position.z
        );

        ma_engine_listener_set_direction(
            &m_engine,
            0,
            direction.x,
            direction.y,
            direction.z
        );

        ma_engine_listener_set_world_up(
            &m_engine,
            0,
            up.x,
            up.y,
            up.z
        );
    }

    void AudioEngine::setMasterVolume(float volume) {
        m_master = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
    }

    float AudioEngine::getMasterVolume() {
        return m_master;
    }

    void AudioEngine::setVolume(SoundType type, float volume) {
        float v = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
        m_volumes[type] = v;
    }

    float AudioEngine::getVolume(SoundType type) {
        auto it = m_volumes.find(type);
        return it == m_volumes.end() ? 1.0f : it->second;
    }

    void AudioEngine::play(Sound* sound) {
        if(!(sound && sound->isValid())) return;

        ma_sound_set_looping(&(sound->getSound()), sound->getLoop());
        float typeVolume = getVolume(sound->getType());
        float finalVolume = sound->getVolume() * typeVolume * m_master;
        ma_sound_set_volume(&(sound->getSound()), finalVolume);

        if(ma_sound_start(&(sound->getSound())) != MA_SUCCESS) {
            FLUXGL_LOG_ERROR("Failed to start sound");
            return;
        }
    }

    bool AudioEngine::isPlaying(Sound* sound) {
        if(!(sound && sound->isValid())) return false;

        return ma_sound_is_playing(&(sound->getSound()));
    }

    void AudioEngine::stop(Sound* sound) {
        if(!(sound && sound->isValid())) return;

        ma_sound_stop(&(sound->getSound()));
    }
}