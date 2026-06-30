#include <fluxgl/audio/sound.h>
#include <fluxgl/core/error.h>

#include <dr_wav.h>

namespace fluxgl {
    Sound Sound::loadFromMemory(Buffer data, AudioFormat fmt) {
        Sound sound;

        switch(fmt) {
            default:
            case AudioFormat::WAV:
                sound.m_format = AudioFormat::WAV;
                sound.decodeWAV(data);
                break;
        }

        return sound;
    }

    void Sound::decodeWAV(Buffer data) {
        drwav wav;

        if(!drwav_init_memory(&wav, data.data(), data.size(), nullptr)) {
            throw Error{ErrorCode::MemError, "could not initialize wav decoder buffer"};
        }

        m_sampleRate = wav.sampleRate;
        m_channels = wav.channels;
        uint64_t frameCount = wav.totalPCMFrameCount;

        m_samples.resize(frameCount * m_channels);
        drwav_read_pcm_frames_f32(
            &wav,
            frameCount,
            m_samples.data()
        );

        drwav_uninit(&wav);
    }
}