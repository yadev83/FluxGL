#pragma once

#include <string>

namespace fluxgl {
    enum class SoundType {
        SFX,
        UI,
        BGM
    };

    struct Sound {
        std::string path;
        SoundType type;

        bool loop = false;
        bool spatialized = true;
        float volume = 1.0f;
    };
}