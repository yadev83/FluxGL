#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <fluxgl/assets/resource_manager.h>
#include "shader.h"
#include "texture.h"

namespace fluxgl {
    struct Material {
        ShaderHandle shader;

        glm::vec3 albedoColor = glm::vec3(1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f);
        glm::vec3 emissionColor = glm::vec3(0.0f);
        float shininess = 32.0f;

        std::vector<TextureHandle> albedoTextures;
        TextureHandle normalMap;
        TextureHandle specularMap;
        TextureHandle emissionMap;
    };
}