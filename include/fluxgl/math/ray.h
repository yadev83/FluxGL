#pragma once

#include <glm/glm.hpp>

namespace fluxgl {
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };
}