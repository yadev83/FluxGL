#include <fluxgl/audio/sound.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/error.h>

#include <miniaudio.h>

namespace fluxgl {
    void Sound::load(const char *data, size_t dataSize, ma_engine *engine) {
        ma_result result = ma_decoder_init_memory(
            data,
            dataSize,
            nullptr,
            &m_decoder
        );

        if(result != MA_SUCCESS) {
            throw Error{ErrorCode::IOError, "decoder init failed"};
        }

        result = ma_sound_init_from_data_source(
            engine,
            &m_decoder,
            0,
            nullptr,
            &m_sound
        );

        if(result != MA_SUCCESS) {
            ma_decoder_uninit(&m_decoder);
            throw Error{ErrorCode::IOError, "sound init failed"};
        }

        m_initialized = true;
    }

    Sound::~Sound() {
        if(m_initialized) {
            ma_sound_uninit(&m_sound);
            ma_decoder_uninit(&m_decoder);
        }
    }

    Sound::Sound(Sound&& other) noexcept {
        m_initialized = other.m_initialized;
        m_decoder = other.m_decoder;
        m_sound = other.m_sound;

        other.m_initialized = false;
    }

    Sound& Sound::operator=(Sound&& other) noexcept {
        if (this != &other) {
            if(m_initialized) {
                ma_sound_uninit(&m_sound);
                ma_decoder_uninit(&m_decoder);
            }
            
            m_initialized = other.m_initialized; 
            m_decoder = other.m_decoder;
            m_sound = other.m_sound;
            
            other.m_initialized = false;
        }
        return *this;
    }

    Sound Sound::loadFromMemory(Buffer data, ma_engine *engine) {
        Sound sound;

        sound.load(
            reinterpret_cast<const char*>(data.data()),
            data.size(), 
            engine
        );
        return sound;
    }
}