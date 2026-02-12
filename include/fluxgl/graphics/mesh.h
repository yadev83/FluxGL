#pragma once

#include <vector>
#include "vertex.h"

namespace fluxgl {
    class Mesh {
        private:
            unsigned int mVAO = 0; 
            unsigned int mVBO = 0;
            unsigned int mEBO = 0;
            size_t mVerticesCount, mIndexCount;

            void build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        public:
            Mesh() = default;
            ~Mesh();
            void draw() const;

            // Builders
            static Mesh fromVertices(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices = {}
            );
            static Mesh quad();
    };
}