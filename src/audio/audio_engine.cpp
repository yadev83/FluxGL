#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/audio/sound.h>
#include <fluxgl/audio/source.h>

#include <fluxgl/core/log.h>

namespace fluxgl {
    AudioEngine& AudioEngine::get() {
        static AudioEngine instance;
        return instance;
    }

    bool AudioEngine::init() {
        if(ma_engine_init(NULL, &m_engine) == MA_SUCCESS) {
            return true;
        } else {
            FLUXGL_LOG_ERROR("Failed to initialize audio engine");
            return false;
        }
    }

    void AudioEngine::shutdown() {
        for(auto& [id, source]: m_sources) {
            ma_sound_uninit(&source.sound);
        }

        ma_engine_uninit(&m_engine);
    }

    void AudioEngine::update() {
        for(auto it = m_sources.begin(); it != m_sources.end(); ) {
            if(!ma_sound_is_playing(&it->second.sound)) {
                ma_sound_uninit(&it->second.sound);
                it = m_sources.erase(it);
            } else {
                ++it;
            }
        }
    }

    void AudioEngine::setSourcePosition(SourceID id, const glm::vec3& position) {
        auto it = m_sources.find(id);
        if(it == m_sources.end()) return;

        ma_sound_set_position(&it->second.sound, position.x, position.y, position.z);
    }

    void AudioEngine::setListener(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {
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

    SoundID AudioEngine::loadSound(std::string path, SoundType type) {
        SoundID id = m_nextSoundID++;
        m_sounds[id] = Sound(path, type);
        return id;
    }

    SourceID AudioEngine::play(SoundID id, bool loop, float volume) {
        auto it = m_sounds.find(id);
        if(it == m_sounds.end()) return 0;

        SourceID sid = m_nextSourceID++;
        Source& source = m_sources[sid];
        
        source.volume = volume;
        source.loop = loop;

        ma_uint32 flags = 0;

        if(it->second.type == SoundType::BGM) {
            flags |= MA_SOUND_FLAG_STREAM;
        } else {
            flags |= MA_SOUND_FLAG_DECODE;
        }

        if(ma_sound_init_from_file(
            &m_engine,
            it->second.path.c_str(),
            flags,
            nullptr,
            nullptr,
            &source.sound
        ) != MA_SUCCESS) {
            FLUXGL_LOG_ERROR("Failed to load sound: " + it->second.path);
            m_sources.erase(sid);
            return 0;
        }

        ma_sound_set_looping(&source.sound, loop);
        ma_sound_set_volume(&source.sound, volume);

        if(ma_sound_start(&source.sound) != MA_SUCCESS) {
            FLUXGL_LOG_ERROR("Failed to start sound: " + it->second.path);
            ma_sound_uninit(&source.sound);
            m_sources.erase(sid);
            return 0;
        }

        return sid;
    }

    bool AudioEngine::isPlaying(SourceID id) {
        auto it = m_sources.find(id);
        if(it == m_sources.end()) return false;
        return ma_sound_is_playing(&it->second.sound);
    }

    void AudioEngine::stop(SourceID id) {
        auto it = m_sources.find(id);
        if(it == m_sources.end()) return;

        ma_sound_stop(&it->second.sound);
        ma_sound_uninit(&it->second.sound);
        m_sources.erase(it);
    }
}