#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "shader.h"
#include "texture.h"

namespace fluxgl {
    struct Material {
        Shader shader;

        glm::vec3 albedoColor = glm::vec3(1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f);
        glm::vec3 emissionColor = glm::vec3(0.0f);
        float shininess = 32.0f;

        std::vector<Texture> albedoTextures;
        Texture normalMap;
        Texture specularMap;
        Texture emissionMap;
    };
}