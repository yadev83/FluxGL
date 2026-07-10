#pragma once

#include <glm/glm.hpp>

namespace fluxgl {
    struct AABB {
        glm::vec2 min = {0.0f, 0.0f};
        glm::vec2 max = {0.0f, 0.0f};

        bool contains(const glm::vec2 point) const;
        bool intersects(const AABB& other) const;
    };
}
