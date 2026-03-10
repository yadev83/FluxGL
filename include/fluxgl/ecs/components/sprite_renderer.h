#pragma once

#include <fluxgl/graphics/texture.h>
#include <glm/glm.hpp>

namespace fluxgl {
    struct SpriteRenderer {
        Texture texture;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        glm::vec2 size = {1.0f, 1.0f};

        int layer = 0;
        glm::vec2 uvMin = {0.0f, 0.0f};
        glm::vec2 uvMax = {1.0f, 1.0f};
    };
}