#pragma once

#include <string>
#include <fluxgl/core/virtual_file_system.h>
#include <miniaudio.h>

namespace fluxgl {
    enum class SoundType {
        SFX,
        UI,
        BGM
    };

    class Sound {
        private:
            SoundType m_type = SoundType::SFX;

            ma_decoder m_decoder;
            ma_sound m_sound;

            bool m_initialized = false;
            float m_volume = 1.0f;
            float m_loop = false;
            float m_spatialized = false;

            void load(const char *data, size_t dataSize, ma_engine *engine);
        
        public:
            Sound() = default;
            ~Sound();

            bool isValid() const { return m_initialized; }
            inline ma_sound& getSound() { return m_sound; }

            inline SoundType getType() const { return m_type; }
            inline float getVolume() const { return m_volume; }
            inline float getLoop() const { return m_loop; }
            inline float getSpatialized() const { return m_spatialized; }

            // Delete copy constructor and copy assignment operator to prevent copying 
            Sound(const Sound&) = delete; 
            Sound& operator=(const Sound&) = delete;
            // Instead, allow move
            Sound(Sound&& other) noexcept;
            Sound& operator=(Sound&& other) noexcept;

            // Builder
            static Sound loadFromMemory(Buffer data, ma_engine *engine);
    };
}