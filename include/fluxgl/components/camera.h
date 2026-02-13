#pragma once

#include "transform.h"

namespace fluxgl {
    struct Camera {
        Transform transform;
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        int width = 800;
        int height = 600;
        bool isOrthographic = false;

        glm::mat4 getViewMatrix() const {
            glm::mat4 model = transform.getModelMatrix();
            return glm::inverse(model);
        }

        glm::mat4 getProjectionMatrix() const {
            float aspectRatio = static_cast<float>(width) / height;
            if (isOrthographic) {
                return glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), nearPlane, farPlane);
            } else {
                return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
            }
        }
    };
}