#pragma once

#include <glm/glm.hpp>

namespace fluxgl {
    enum class LightType { Ambient, Directional, Point };

    struct Light {
        LightType type = LightType::Ambient;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
    };
}