#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <fluxgl/assets/assets.h>
#include "shader.h"
#include "texture.h"

namespace fluxgl {
    struct Material {
        Resource shader;

        glm::vec3 albedoColor = glm::vec3(1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f);
        glm::vec3 emissionColor = glm::vec3(0.0f);
        float shininess = 32.0f;

        std::vector<Resource> albedoTextures;
        Resource normalMap;
        Resource specularMap;
        Resource emissionMap;
    };
}