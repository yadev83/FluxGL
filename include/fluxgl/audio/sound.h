#pragma once

#include <fluxgl/core/virtual_file_system.h>

namespace fluxgl {
    enum class AudioFormat {
        WAV
    };

    class Sound {
        private:
            uint32_t    m_sampleRate;
            uint16_t    m_channels;
            AudioFormat m_format;

            std::vector<float> m_samples;

            void decodeWAV(Buffer data);
        public:
            Sound() = default;

            uint32_t getChannels() const { return m_channels; }
            uint64_t getFrameCount() const { return (m_samples.size() / m_channels); }
            const std::vector<float>& getSamples() const { return m_samples; }

            
            // Delete copy constructor and copy assignment operator to prevent copying 
            Sound(const Sound&) = delete; 
            Sound& operator=(const Sound&) = delete;
            // Instead, allow move
            Sound(Sound&& other) noexcept = default;
            Sound& operator=(Sound&& other) noexcept = default;

            // Builders
            static Sound loadFromMemory(Buffer data, AudioFormat fmt = AudioFormat::WAV); 
    };
}