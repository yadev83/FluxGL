#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace fluxgl {
    class Shader;

    struct DebugVertex {
        glm::vec3 position;
        glm::vec3 color;
    };

    class DebugRenderer {
        private:
            static bool m_enabled;

            static std::vector<DebugVertex> m_verticesQueue;
            static unsigned int m_vao;
            static unsigned int m_vbo;
            static Shader m_shader;

            static glm::mat4 m_view;
            static glm::mat4 m_projection;

        public:
            static void enable();
            static void disable();
            static bool isEnabled();
            static void setCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

            static void line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color);
            static void rect(const glm::vec2& min, const glm::vec2& max, const glm::vec3& color);
            static void dot(const glm::vec2& position, const glm::vec3& color);
            static void cross(const glm::vec2& position, const glm::vec3& color);
            static void flush();
    };
}