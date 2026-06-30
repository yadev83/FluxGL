#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    AudioEngine::AudioEngine() {
        if(ma_engine_init(nullptr, &m_engine) != MA_SUCCESS) {
            throw Error{ErrorCode::AudioEngineError, "failed to create audio engine\n"};
        }

        FLUXGL_LOG_INFO("Initializing AudioEngine...");
    }

    AudioEngine::~AudioEngine() {
        for (auto& [id, voice] : m_voices) {
            ma_sound_uninit(&voice->sound);
            ma_audio_buffer_uninit(&voice->buffer);
        }

        m_voices.clear();
        ma_engine_uninit(&m_engine);
    }

    bool AudioEngine::isValid(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        return it != m_voices.end();
    }

    void AudioEngine::cleanupUnusedVoices() {
        for (auto it = m_voices.begin(); it != m_voices.end();) {
            AudioVoiceHandle handle = it->first;
            AudioVoice* voice = it->second;

            if (voice->shouldStop || ma_sound_at_end(&voice->sound)) {
                ma_sound_uninit(&voice->sound);
                ma_audio_buffer_uninit(&voice->buffer);

                it = m_voices.erase(it);
                FLUXGL_LOG_TRACE("Clearing sound voice " + std::to_string(handle));
            } else {
                ++it;
            }
        }
    }

    AudioVoiceHandle AudioEngine::play(const Sound* sound, AudioVoiceSettings settings) {
        AudioVoice* voice = new AudioVoice();

        ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(ma_format_f32, sound->getChannels(), sound->getFrameCount(), sound->getSamples().data(), nullptr);

        if(ma_audio_buffer_init(&bufferConfig, &(voice->buffer)) != MA_SUCCESS) {
            throw Error{ErrorCode::AudioEngineError, "failed to load audio buffer"};
        }

        if(ma_sound_init_from_data_source(&m_engine, &(voice->buffer), 0, nullptr, &(voice->sound)) != MA_SUCCESS ) {
            throw Error{ErrorCode::AudioEngineError, "failed to init sound instance from audio buffer"};
        }

        ma_sound_set_volume(&(voice->sound), settings.volume);
        ma_sound_set_looping(&(voice->sound), settings.loop);
        ma_sound_start(&(voice->sound));

        AudioVoiceHandle handle = m_nextHandle++;
        m_voices.emplace(
            handle,
            voice
        );

        FLUXGL_LOG_TRACE("Creating audio voice " + std::to_string(handle));
        return handle;
    }

    void AudioEngine::pause(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        ma_sound_stop(&it->second->sound);
        it->second->isPaused = true;

        FLUXGL_LOG_TRACE("Pausing audio voice " + std::to_string(it->first));
    }

    void AudioEngine::resume(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        ma_sound_start(&it->second->sound);
        it->second->isPaused = false;

        FLUXGL_LOG_TRACE("Resuming audio voice " + std::to_string(it->first));
    }

    void AudioEngine::stop(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        // Go to end for cleanup to detect it as stopped
        ma_sound_stop(&it->second->sound);
        it->second->shouldStop = true;

        FLUXGL_LOG_TRACE("Stopping audio voice " + std::to_string(it->first));
    }

    bool AudioEngine::isPlaying(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return false;

        return ma_sound_is_playing(&it->second->sound);
    }
}