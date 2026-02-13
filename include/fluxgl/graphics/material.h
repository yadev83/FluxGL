#pragma once

#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"

namespace fluxgl {
    struct Material {
        Shader shader;
        Texture texture;
        glm::vec3 color = glm::vec3(1.0f);
    };
}