#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "shader.h"
#include "texture.h"

namespace fluxgl {
    struct Material {
        Shader shader;

        glm::vec3 albedoColor = glm::vec3(1.0f);
        glm::vec3 emissiveColor = glm::vec3(0.0f);
        float metallic = 1.0f;
        float roughness = 1.0f;
        float ambientOcclusion = 1.0f;

        std::vector<Texture> albedoTextures;
        Texture normalMap;
        Texture metallicMap;
        Texture roughnessMap;
        Texture ambientOcclusionMap;
        Texture emissiveMap;
    };
}