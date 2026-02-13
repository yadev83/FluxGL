#include <fluxgl/graphics/renderer.h>
#include <fluxgl/core/log.h>
#include <glad/glad.h>

namespace fluxgl {
    void Renderer::clear(const glm::vec3& color) {
        glClearColor(color.r, color.g, color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw(const Mesh& mesh, const Material& material) {
        // Binding shader
        if(material.shader.isValid()) material.shader.bind();

        // Binding textures/maps
        int slot = 0;
        for(size_t i = 0; i < material.albedoTextures.size(); i++) {
            if(material.albedoTextures[i].isValid()) {
                material.albedoTextures[i].bind(slot);
                material.shader.setUniform("u_Albedo[" + std::to_string(slot) + "]", slot);
                slot++;
            }
        }
        material.shader.setUniform("u_AlbedoTextureCount", slot);

        if(material.normalMap.isValid()) {
            material.normalMap.bind(slot);
            material.shader.setUniform("u_NormalMap", slot);
            slot++;
        }
        if(material.metallicMap.isValid()) {
            material.metallicMap.bind(slot);
            material.shader.setUniform("u_MetallicMap", slot);
            slot++;
        }
        if(material.roughnessMap.isValid()) {
            material.roughnessMap.bind(slot);
            material.shader.setUniform("u_RoughnessMap", slot);
            slot++;
        }
        if(material.ambientOcclusionMap.isValid()) {
            material.ambientOcclusionMap.bind(slot);
            material.shader.setUniform("u_AmbientOcclusionMap", slot);
            slot++;
        }   
        if(material.emissiveMap.isValid()) {
            material.emissiveMap.bind(slot);
            material.shader.setUniform("u_EmissiveMap", slot);
            slot++;
        }

        // Binding colors
        material.shader.setUniform("u_AlbedoColor", material.albedoColor);
        material.shader.setUniform("u_EmissiveColor", material.emissiveColor);
        // Binding material properties
        material.shader.setUniform("u_Metallic", material.metallic);
        material.shader.setUniform("u_Roughness", material.roughness);
        material.shader.setUniform("u_AmbientOcclusion", material.ambientOcclusion);

        // DRAW CALL
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

        // Unbinding textures/maps
        slot = 0;
        for(size_t i = 0; i < material.albedoTextures.size(); i++) {
            if(material.albedoTextures[i].isValid()) {
                material.albedoTextures[i].unbind(slot++);
            }
        }
        if(material.normalMap.isValid()) material.normalMap.unbind(slot++);
        if(material.metallicMap.isValid()) material.metallicMap.unbind(slot++);
        if(material.roughnessMap.isValid()) material.roughnessMap.unbind(slot++);
        if(material.ambientOcclusionMap.isValid()) material.ambientOcclusionMap.unbind(slot++);
        if(material.emissiveMap.isValid()) material.emissiveMap.unbind(slot++);
        
        // Unbinding shader
        if(material.shader.isValid()) material.shader.unbind();
    }
}