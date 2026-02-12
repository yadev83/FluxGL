#pragma once

#include <glm/glm.hpp>
#include "shader.h"

namespace fluxgl {
    struct Material {
        Shader shader;
        glm::vec3 color = glm::vec3(1.0f);
    };
}