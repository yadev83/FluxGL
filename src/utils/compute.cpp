#include <fluxgl/utils/compute.h>

namespace fluxgl {
    glm::vec3 rotationToDirection(const glm::vec3& rotDeg) {
        glm::vec3 rot = glm::radians(rotDeg);
        float pitch = rot.x;
        float yaw = rot.y;

        glm::vec3 dir;
        dir.x = cos(pitch) * cos(yaw);
        dir.y = sin(pitch);
        dir.z = cos(pitch) * sin(yaw);

        return glm::normalize(dir);
    }
}