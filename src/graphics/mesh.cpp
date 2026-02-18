#include <fluxgl/graphics/mesh.h>
#include <fluxgl/core/log.h>

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

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
        if(m_EBO || m_VBO || m_VAO) {
            FLUXGL_LOG_DEBUG("Destroying mesh (VAO ID: " + std::to_string(m_VAO) + ")");
            if(m_EBO) glDeleteBuffers(1, &m_EBO); 
            if(m_VBO) glDeleteBuffers(1, &m_VBO); 
            if(m_VAO) glDeleteVertexArrays(1, &m_VAO); 
        }
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

    Mesh Mesh::cube() {
        std::vector<Vertex> vertices = {
            // ===== Face +Z (Front) =====
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}},

            // ===== Face -Z (Back) =====
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}},

            // ===== Face +X (Right) =====
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}},

            // ===== Face -X (Left) =====
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}},

            // ===== Face +Y (Top) =====
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}},

            // ===== Face -Y (Bottom) =====
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 0.f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 0.f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}},
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.f, 1.f}}
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,   // +Z
            4, 5, 6, 6, 7, 4,   // -Z
            8, 9,10,10,11, 8,   // +X
            12,13,14,14,15,12,  // -X
            16,17,18,18,19,16,  // +Y
            20,21,22,22,23,20   // -Y
        };
        
        return Mesh::fromVertices(vertices, indices);
    }

    Mesh Mesh::sphere(size_t stacks, size_t sectors) {
        std::vector<Vertex> vertices = {};
        std::vector<unsigned int> indices = {};

        for(int i = 0; i <= stacks; i++) {
            float v = (float)i / stacks;
            float phi = v * glm::pi<float>();

            for (int j = 0; j <= sectors; j++) {
                float u = (float)j / sectors;
                float theta = u * glm::two_pi<float>();

                float x = 0.5f * std::cos(theta) * std::sin(phi);
                float y = 0.5f * std::cos(phi);
                float z = 0.5f * std::sin(theta) * std::sin(phi);

                vertices.push_back({
                    {x, y, z}, {1.0f, 1.0f, 1.0f}, {u, v}
                });
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < sectors; ++j) {
                int first  = i * (sectors + 1) + j;
                int second = first + sectors + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
        
        return Mesh::fromVertices(vertices, indices);
    }

    // MOVE-ONLY
    Mesh::Mesh(Mesh&& other) noexcept {
        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_EBO = other.m_EBO;
        m_verticesCount = other.m_verticesCount;
        m_indexCount = other.m_indexCount;

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_EBO = 0;
        other.m_verticesCount = 0;
        other.m_indexCount = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (this != &other) {
            if(m_VAO) {
                glDeleteVertexArrays(1, &m_VAO);
            }
            
            m_VAO = other.m_VAO;
            m_VBO = other.m_VBO;
            m_EBO = other.m_EBO;
            m_verticesCount = other.m_verticesCount;
            m_indexCount = other.m_indexCount;

            other.m_VAO = 0;
            other.m_VBO = 0;
            other.m_EBO = 0;
            other.m_verticesCount = 0;
            other.m_indexCount = 0;
        }
        return *this;
    }
}