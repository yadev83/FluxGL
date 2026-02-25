#pragma once

#include <glm/glm.hpp>
#include <miniaudio.h>

#include <unordered_map>
#include <string>

namespace fluxgl {
    using SoundID = unsigned int;
    using SourceID = unsigned int;

    struct Sound;
    enum class SoundType;
    struct Source;
    
    class AudioEngine {
        private:
            ma_engine m_engine{};
            
            std::unordered_map<SoundID, Sound> m_sounds;
            SoundID m_nextSoundID = 1;
            std::unordered_map<SourceID, Source> m_sources;
            SourceID m_nextSourceID = 1;

            float m_master = 1.0f;
            std::unordered_map<SoundType, float> m_volumes;

            AudioEngine() = default;

        public:
            static AudioEngine& get();

            bool init();
            void shutdown();
            void update();

            void setSourcePosition(SourceID id, const glm::vec3& position);
            void setSourceSpatialized(SourceID id, bool spatialized);
            void setListener(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

            void setMasterVolume(float volume);
            float getMasterVolume();
            void setVolume(SoundType type, float volume);
            float getVolume(SoundType type);

            SoundID loadSound(std::string path, SoundType type);
            SourceID play(SoundID id, bool loop = false, float volume = 1.0f);
            bool isPlaying(SourceID id);
            void stop(SourceID id);
    };
}