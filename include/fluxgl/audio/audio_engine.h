#pragma once

#include <glm/glm.hpp>
#include <miniaudio.h>

#include <unordered_map>
#include <string>

namespace fluxgl {
    using SoundID = unsigned int;
    
    class AudioEngine {
        private:
            ma_engine m_engine{};
            std::unordered_map<SoundID, std::string> m_paths;
            SoundID m_nextID = 1;

            AudioEngine() = default;

        public:
            static AudioEngine& get();

            bool init();
            void shutdown();

            void setListener(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

            SoundID loadSound(std::string path);
            void play(SoundID id);
    };
}