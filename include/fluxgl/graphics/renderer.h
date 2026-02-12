#pragma once

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"

namespace fluxgl {
    class Renderer {
        private:
            Renderer() = default;
            ~Renderer() = default;

        public:
            static void clear(const glm::vec4& color);
            static void draw(const Mesh& mesh, const Shader& shader);
    };
}