#include <fluxgl/graphics/renderer.h>
#include <glad/glad.h>

namespace fluxgl {
    void Renderer::clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw(const Mesh& mesh, const Shader& shader) {
        shader.bind();

        unsigned int vao = mesh.getVAO();
        size_t indexCount = mesh.getIndexCount();
        size_t verticesCount = mesh.getVerticesCount();

        if (vao > 0) { 
            glBindVertexArray(vao); 
            if (indexCount > 0) { 
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); 
            } else { 
                glDrawArrays(GL_TRIANGLES, 0, verticesCount); 
            } 
        }
        
        glBindVertexArray(0); // Unbind after drawing
        shader.unbind();
    }
}