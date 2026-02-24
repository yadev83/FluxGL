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
        SoundType type = SoundType::SFX;
    };
}