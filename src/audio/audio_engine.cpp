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
            std::move(voice)
        );

        return handle;
    }

    void AudioEngine::pause(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        ma_sound_stop(&it->second->sound);
    }

    void AudioEngine::resume(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        ma_sound_start(&it->second->sound);
    }

    void AudioEngine::stop(AudioVoiceHandle handle) {
        auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return;

        ma_sound_stop(&it->second->sound);

        ma_sound_uninit(&it->second->sound);
        ma_audio_buffer_uninit(&it->second->buffer);

        m_voices.erase(it);
    }

    bool AudioEngine::isPlaying(AudioVoiceHandle handle) {
         auto it = m_voices.find(handle);
        if (it == m_voices.end())
            return false;

        return ma_sound_is_playing(&it->second->sound);
    }
}