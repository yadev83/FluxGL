#include <fluxgl/math/aabb.h>

namespace fluxgl {
    bool AABB::contains(const glm::vec2 point) const {
        return 
            point.x >= min.x &&
            point.x <= max.x &&
            point.y >= min.y &&
            point.y <= max.y;
    }

    bool AABB::intersects(const AABB& other) const {
        return
            min.x <= other.max.x &&
            max.x >= other.min.x &&
            min.y <= other.max.y &&
            max.y >= other.min.y;
    }
}