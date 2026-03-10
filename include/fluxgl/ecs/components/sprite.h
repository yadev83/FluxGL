#pragma once

#include <fluxgl/graphics/texture.h>
#include <fluxgl/graphics/shader.h>

namespace fluxgl {
    struct Sprite {
        Texture texture;
        Shader shader;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        glm::vec2 size = {1.0f, 1.0f};

        int layer = 0;
        glm::vec2 uvMin = {0.0f, 0.0f};
        glm::vec2 uvMax = {1.0f, 1.0f};
    };
}