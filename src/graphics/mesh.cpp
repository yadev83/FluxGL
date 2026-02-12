#include <fluxgl/graphics/mesh.h>
#include <fluxgl/core/log.h>

#include <glad/glad.h>

namespace fluxgl {
    void Mesh::build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        m_verticesCount = vertices.size();
        m_indexCount = indices.size();

        // VAO generation + Bind
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // VBO generation + Bind + Data
        glGenBuffers(1, &m_VBO); 
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER, 
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW
        );

        // Vertex attribute setup
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); 
        glEnableVertexAttribArray(1); 
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        // EBO Generation + Bind + Data (if indices are provided) 
        if (!indices.empty()) { 
            glGenBuffers(1, &m_EBO); 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); 
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER, 
                indices.size() * sizeof(unsigned int), 
                indices.data(), 
                GL_STATIC_DRAW 
            ); 
        } 
        
        // Unbind VAO (optional but good practice) 
        glBindVertexArray(0);
        FLUXGL_LOG_DEBUG("Mesh built (VAO ID: " + std::to_string(m_VAO) + ")");
    }
    
    Mesh::~Mesh() {
        FLUXGL_LOG_DEBUG("Destroying mesh (VAO ID: " + std::to_string(m_VAO) + ")");
        if(m_EBO) glDeleteBuffers(1, &m_EBO); 
        if(m_VBO) glDeleteBuffers(1, &m_VBO); 
        if(m_VAO) glDeleteVertexArrays(1, &m_VAO); 
    }

    unsigned int Mesh::getVAO() const {
        return m_VAO;
    }

    size_t Mesh::getVerticesCount() const {
        return m_verticesCount;
    }

    size_t Mesh::getIndexCount() const { 
        return m_indexCount; 
    }

    Mesh Mesh::fromVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        Mesh mesh;
        mesh.build(vertices, indices);
        return mesh;
    }

    Mesh Mesh::quad() {
        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };
        
        return Mesh::fromVertices(vertices, indices);
    }
}