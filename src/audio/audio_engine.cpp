#include <fluxgl/audio/audio_engine.h>

namespace fluxgl {
    AudioEngine& AudioEngine::get() {
        static AudioEngine instance;
        return instance;
    }

    bool AudioEngine::init() {
        return ma_engine_init(NULL, &m_engine) == MA_SUCCESS;
    }

    void AudioEngine::shutdown() {
        ma_engine_uninit(&m_engine);
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

    SoundID AudioEngine::loadSound(std::string path) {
        SoundID id = m_nextID++;
        m_paths[id] = path;
        return id;
    }

    void AudioEngine::play(SoundID id) {
        if(!m_paths.contains(id)) return;

        ma_engine_play_sound(&m_engine, m_paths[id].c_str(), nullptr);
    }
}