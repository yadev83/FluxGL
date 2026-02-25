#pragma once

#include <miniaudio.h>

namespace fluxgl {
    struct Source {
        ma_sound sound;
        float volume = 1.0f;
        float loop = false;
    };
}