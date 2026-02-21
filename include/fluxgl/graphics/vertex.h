#pragma once

#include <glm/glm.hpp>

namespace fluxgl {
    struct Vertex {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 normal = {0.0f, 0.0f, 0.0f};
        glm::vec3 color = {0.0f, 0.0f, 0.0f};
        glm::vec2 uv = {0.0f, 0.0f};
    };
}