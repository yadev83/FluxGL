#pragma once

#include <string>
#include <fluxgl/audio/source.h>
#include <fluxgl/audio/sound.h>

namespace fluxgl {
    using SourceID = unsigned int;

    struct AudioSource {
        SourceID source = 0;
        Sound sound;

        bool loop = false;
        bool spatialized = true;
        float volume = 1.0f;

        bool shouldPlay = false;
        bool shouldStop = false;
    };
}