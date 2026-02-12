#include <fluxgl/graphics/mesh.h>

#include <glad/glad.h>

namespace fluxgl {
    void Mesh::build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        mVerticesCount = vertices.size();
        mIndexCount = indices.size();

        // VAO generation + Bind
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        // VBO generation + Bind + Data
        glGenBuffers(1, &mVBO); 
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
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
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        // EBO Generation + Bind + Data (if indices are provided) 
        if (!indices.empty()) { 
            glGenBuffers(1, &mEBO); 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO); 
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER, 
                indices.size() * sizeof(unsigned int), 
                indices.data(), 
                GL_STATIC_DRAW 
            ); 
        } 
        
        // Unbind VAO (optional but good practice) 
        glBindVertexArray(0);
    }
    
    Mesh::~Mesh() { 
        if(mEBO) glDeleteBuffers(1, &mEBO); 
        if(mVBO) glDeleteBuffers(1, &mVBO); 
        if(mVAO) glDeleteVertexArrays(1, &mVAO); 
    } 
    
    void Mesh::draw() const { 
        glBindVertexArray(mVAO); 
        
        if (mIndexCount > 0) { 
            glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0); 
        } else { 
            glDrawArrays(GL_TRIANGLES, 0, mVerticesCount); 
        } 
        
        glBindVertexArray(0); // Unbind after drawing
    }

    Mesh Mesh::fromVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        Mesh mesh;
        mesh.build(vertices, indices);
        return mesh;
    }

    Mesh Mesh::quad() {
        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };
        
        Mesh mesh;
        mesh.build(vertices, indices);
        return mesh;
    }
}