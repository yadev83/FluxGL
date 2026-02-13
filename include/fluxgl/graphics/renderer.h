#pragma once

#include <glm/glm.hpp>
#include <fluxgl/components/renderable.h>
#include <fluxgl/components/camera.h>

namespace fluxgl {
    class Renderer {
        private:
            Renderer() = default;
            ~Renderer() = default;

        public:
            static void clear(const glm::vec3& color = {0.1f, 0.1f, 0.1f});
            static void draw(const Renderable& renderable, const Camera* camera = nullptr);
    };
}