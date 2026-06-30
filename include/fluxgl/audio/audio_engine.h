#pragma once

#include <miniaudio.h>
#include <fluxgl/audio/sound.h>
#include <unordered_map>

namespace fluxgl {
    using AudioVoiceHandle = uint32_t;

    struct AudioVoice {
        ma_audio_buffer buffer;
        ma_sound sound;
    };

    struct AudioVoiceSettings {
        float volume = 1.0f;
        bool loop = false;
        };

    class AudioEngine {
        private:
            ma_engine m_engine;
            AudioVoiceHandle m_nextHandle;
            std::unordered_map<AudioVoiceHandle, AudioVoice*> m_voices;

        public:
            AudioEngine();
            ~AudioEngine();

            AudioVoiceHandle play(const Sound* sound, AudioVoiceSettings settings = {1.0f, false});
            void pause(AudioVoiceHandle handle);
            void resume(AudioVoiceHandle handle);
            void stop(AudioVoiceHandle handle);
            bool isPlaying(AudioVoiceHandle handle);
    };
}