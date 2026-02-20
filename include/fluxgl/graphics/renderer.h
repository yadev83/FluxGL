#pragma once

#include <glm/glm.hpp>
#include "mesh.h"
#include "material.h"

namespace fluxgl {
    class Renderer {
        private:
            Renderer() = default;
            ~Renderer() = default;

        public:
            static void clear(const glm::vec3& color = {0.1f, 0.1f, 0.1f});
            static void drawMesh(
                const Mesh& mesh, 
                const Material& material,

                glm::mat4 modelMatrix = glm::mat4(1.0f), 
                glm::mat4 viewMatrix = glm::mat4(1.0f), 
                glm::mat4 projectionMatrix = glm::mat4(1.0f));
    };
}