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
    
    /**
     * @brief Audio Engine core class for FluxGL
     * 
     * Describes a simple audio engine for FluxGL by using Miniaudio through a singleton class implementation.
     * The engine must be initialized before use and properly shutdown when done. It allows for loading and playing sounds.
     * Contains a master volume setting and a volume setting per sound type, along with spatialization and listener settings.
     */
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
            /**
             * @brief Get the AudioEngine singleton instance
             * 
             * @return AudioEngine& 
             */
            static AudioEngine& get();
            static ma_engine *getEngine();

            /**
             * @brief Initialize the audio engine
             * 
             * Starts the Miniaudio engine and setup the class to be ready for use
             * 
             * @return true 
             * @return false 
             */
            bool init();
            /**
             * @brief Shutdown the audio engine
             * 
             * Properly closes the miniaudio lib
             */
            void shutdown();

            /**
             * @brief Set the position of a source for proper spatialization
             * 
             * @param id 
             * @param position 
             */
            void setSoundPosition(Sound* sound = nullptr, const glm::vec3& position = {0.0f, 0.0f, 0.0f});
            /**
             * @brief Set the Listener object
             * 
             * The listener object has a position, a direction, and an up vector
             * With all that, the engine makes sure that all sources are properly spatialized
             * If we want 2D sounds, then sources must not enable spatialization, the listener position is then irrelevant.
             * 
             * @param position 
             * @param direction 
             * @param up 
             */
            void setListenerPosition(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

            void setMasterVolume(float volume);
            float getMasterVolume();
            void setVolume(SoundType type, float volume);
            float getVolume(SoundType type);

            void play(Sound* sound = nullptr);
            bool isPlaying(Sound* sound = nullptr);
            void stop(Sound* sound = nullptr);
    };
}