#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace fluxgl {
    struct Transform {
        glm::vec3 position  = glm::vec3(0.0f);
        glm::vec3 rotation  = glm::vec3(0.0f);
        glm::vec3 scale     = glm::vec3(1.0f);

        glm::mat4 getModelMatrix() const {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 r = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
            glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

            return t * r * s;
        }
    };
}