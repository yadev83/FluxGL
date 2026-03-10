#pragma once

#include "transform.h"

namespace fluxgl {
    struct Camera {
        float fov = 45.0f;

        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        bool isOrthographic = false;
        int worldWidth = 800;
        int worldHeight = 600;

        glm::mat4 getViewMatrix(Transform& transform) const {
            return glm::lookAt(transform.position, transform.position + transform.front(), transform.up());
        }

        glm::mat4 getProjectionMatrix(float aspectRatio = 1.0f) const {
            if (isOrthographic) {
                return glm::ortho(
                    -worldWidth * 0.5f, 
                    worldWidth * 0.5f, 
                    -worldHeight * 0.5f, 
                    worldHeight * 0.5f, 
                    nearPlane, 
                    farPlane
                );
            } else {
                return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
            }
        }
    };
}