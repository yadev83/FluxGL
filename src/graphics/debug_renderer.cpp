#include <fluxgl/graphics/debug_renderer.h>
#include <fluxgl/graphics/shader.h>
#include <fluxgl/core/log.h>

#include <glad/glad.h>

namespace fluxgl {
    bool DebugRenderer::m_enabled;
    Shader DebugRenderer::m_shader;
    unsigned int DebugRenderer::m_vao = 0;
    unsigned int DebugRenderer::m_vbo = 0;
    std::vector<DebugVertex> DebugRenderer::m_verticesQueue;
    glm::mat4 DebugRenderer::m_view = glm::mat4(1.0f);
    glm::mat4 DebugRenderer::m_projection = glm::mat4(1.0f);

    void DebugRenderer::enable() {
        if(!m_shader.isValid()) m_shader = Shader::debugUnlit();
        
        if(!m_vao) {
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            if(!m_vbo) {
                glGenBuffers(1, &m_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

                glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

                // position
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
                // color
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));
            }

            glBindVertexArray(0);
        }

        m_enabled = true;
    }
    
    void DebugRenderer::disable() {
        if(m_vao) glDeleteVertexArrays(1, &m_vao);
        if(m_vbo) glDeleteBuffers(1, &m_vbo);
        m_verticesQueue.clear();

        m_enabled = false;
    }

    bool DebugRenderer::isEnabled() {
        return m_enabled;
    }

    void DebugRenderer::setCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
        DebugRenderer::m_view = viewMatrix;
        DebugRenderer::m_projection = projectionMatrix;
    }

    void DebugRenderer::flush() {
        if(!m_enabled) return;
        if(m_verticesQueue.empty()) return;

        m_shader.bind();
        m_shader.setUniform("u_View", m_view);
        m_shader.setUniform("u_Projection", m_projection);

        // Disable depth testing first
        glDisable(GL_DEPTH_TEST);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, m_verticesQueue.size() * sizeof(DebugVertex), m_verticesQueue.data(), GL_DYNAMIC_DRAW);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINES, 0, m_verticesQueue.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Re-nable depth testing
        glEnable(GL_DEPTH_TEST);

        // Clear queue
        m_verticesQueue.clear();
    }

    void DebugRenderer::line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color) {
        if(!m_enabled) return;

        m_verticesQueue.push_back({a, color});
        m_verticesQueue.push_back({b, color});
    }

    void DebugRenderer::rect(const glm::vec2& min, const glm::vec2& max, const glm::vec3& color) {
        if(!m_enabled) return;

        line({min.x, min.y, 0.0f}, {max.x, min.y, 0.0f}, color);
        line({max.x, min.y, 0.0f}, {max.x, max.y, 0.0f}, color);
        line({max.x, max.y, 0.0f}, {min.x, max.y, 0.0f}, color);
        line({min.x, max.y, 0.0f}, {min.x, min.y, 0.0f}, color);
    }

    void DebugRenderer::dot(const glm::vec2& position, const glm::vec3& color) {
        if(!m_enabled) return;

        line({position.x, position.y, 0.0f}, {position.x, position.y, 0.0f}, color);
    }

    void DebugRenderer::cross(const glm::vec2& position, const glm::vec3& color) {
        if(!m_enabled) return;

        line({position.x, position.y - 0.1f, 0.0f}, {position.x, position.y + 0.1f, 0.0f}, color);
        line({position.x - 0.1f, position.y, 0.0f}, {position.x + 0.1f, position.y, 0.0f}, color);
    }
}