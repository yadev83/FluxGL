#pragma once

#include <glm/glm.hpp>

#include "material.h"
#include "mesh.h"

namespace fluxgl {
    class Renderer {
        private:
            Renderer() = default;
            ~Renderer() = default;

        public:
            static void clear(const glm::vec3& color);
            static void draw(const Mesh& mesh, const Material& material);
    };
}