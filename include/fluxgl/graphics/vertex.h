#pragma once

#include <glm/glm.hpp>

namespace fluxgl {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        Vertex() = default;
        Vertex(const glm::vec3& pos, const glm::vec2& uv) : position(pos), uv(uv) {}
    };
}