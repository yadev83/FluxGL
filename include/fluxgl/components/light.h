#pragma once

#include "transform.h"

namespace fluxgl {
    struct Light {
        Transform transform;
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
    };
}