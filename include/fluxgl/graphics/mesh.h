#pragma once

#include <vector>
#include "vertex.h"

namespace fluxgl {
    class Mesh {
        private:
            unsigned int m_VAO = 0; 
            unsigned int m_VBO = 0;
            unsigned int m_EBO = 0;
            size_t m_verticesCount, m_indexCount;

            void build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        public:
            Mesh() = default;
            ~Mesh();

            // Getters
            unsigned int getVAO() const;
            size_t getVerticesCount() const;
            size_t getIndexCount() const;

            // Builders
            static Mesh fromVertices(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices = {}
            );
            static Mesh quad();
            static Mesh cube();
            static Mesh sphere(size_t stacks = 16, size_t sectors = 32);
            
            // MOVE-ONLY
            // Delete copy constructor and copy assignment operator to prevent copying 
            Mesh(const Mesh&) = delete; 
            Mesh& operator=(const Mesh&) = delete;
            // Instead, allow move
            Mesh(Mesh&& other) noexcept;
            Mesh& operator=(Mesh&& other) noexcept;
    };
}