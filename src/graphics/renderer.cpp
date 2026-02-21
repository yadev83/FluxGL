#include <fluxgl/graphics/renderer.h>
#include <fluxgl/core/log.h>
#include <glad/glad.h>

namespace fluxgl {
    RenderState Renderer::m_state;
    SceneState Renderer::m_sceneState;
    
    void Renderer::setFramebufferSize(int width, int height) {
        float aspectRatio = (float)width / height;
        float targetAspectRatio = m_state.targetAspectRatio;

        m_state.framebufferHeight = height;
        m_state.framebufferWidth = width;

        if(targetAspectRatio != 0.0f) {
            if(aspectRatio > targetAspectRatio) {
                m_state.viewportHeight = height;
                m_state.viewportWidth = int(height * targetAspectRatio);
                m_state.viewportX = (width - m_state.viewportWidth) / 2.0f;
                m_state.viewportY = 0.0f;
            } else {
                m_state.viewportWidth = width;
                m_state.viewportHeight = int(width / targetAspectRatio);
                m_state.viewportX = 0.0f;
                m_state.viewportY = (height - m_state.viewportHeight) / 2.0f;
            }
        }
    }

    void Renderer::beginFrame() {
        // OPENGL STATE RESET
        glViewport(0, 0, m_state.framebufferWidth, m_state.framebufferHeight);

        glDisable(GL_SCISSOR_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_SCISSOR_TEST);
        glScissor(
            m_state.viewportX,
            m_state.viewportY,
            m_state.viewportWidth,
            m_state.viewportHeight
        );
        
        glViewport(
            m_state.viewportX,
            m_state.viewportY,
            m_state.viewportWidth,
            m_state.viewportHeight
        );
        
        glClearColor(m_state.clearColor.r, m_state.clearColor.g, m_state.clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        // SCENE STATE RESET
        m_sceneState.directionalLights.clear();
        m_sceneState.pointLights.clear();
        m_sceneState.ambientLights.clear();
    }

    void Renderer::setCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& position) {
        m_sceneState.viewMatrix = viewMatrix;
        m_sceneState.projectionMatrix = projectionMatrix;
        m_sceneState.cameraPosition = position;
    }

    void Renderer::setClearColor(const glm::vec3& color) {
        m_state.clearColor = color;
    }

    void Renderer::registerAmbientLight(const glm::vec3& lightColor, const float intensity) {
        m_sceneState.ambientLights.push_back(AmbientLight{
            .color = lightColor,
            .intensity = intensity
        });
    }

    void Renderer::registerDirectionalLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& direction) {
        m_sceneState.directionalLights.push_back(DirectionalLight{
            .color = lightColor,
            .intensity = intensity,
            .direction = direction
        });
    }

    void Renderer::registerPointLight(const glm::vec3& lightColor, const float intensity, const glm::vec3& position) {
        m_sceneState.pointLights.push_back(PointLight{
            .color = lightColor,
            .intensity = intensity,
            .position = position
        });
    }

    void Renderer::drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix) {
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
            material.shader.setUniform("u_UseNormalMap", true);
            slot++;
        } else {
            material.shader.setUniform("u_UseNormalMap", false);
        }
        
        if(material.specularMap.isValid()) {
            material.specularMap.bind(slot);
            material.shader.setUniform("u_SpecularMap", slot);
            material.shader.setUniform("u_UseSpecularMap", true);
            slot++;
        } else {
            material.shader.setUniform("u_UseSpecularMap", false);
        }

        if(material.emissionMap.isValid()) {
            material.emissionMap.bind(slot);
            material.shader.setUniform("u_EmissionMap", slot);
            material.shader.setUniform("u_UseEmissionMap", true);
            slot++;
        } else {
            material.shader.setUniform("u_UseEmissionMap", false);
        }

        // Binding colors
        material.shader.setUniform("u_AlbedoColor", material.albedoColor);
        material.shader.setUniform("u_SpecularColor", material.specularColor);
        material.shader.setUniform("u_EmissionColor", material.emissionColor);
        // Binding material properties
        material.shader.setUniform("u_SpecularColor", material.specularColor);
        material.shader.setUniform("u_Shininess", material.shininess);

        // Binding MVP matrices
        material.shader.setUniform("u_View", m_sceneState.viewMatrix);
        material.shader.setUniform("u_Projection", m_sceneState.projectionMatrix);
        material.shader.setUniform("u_Model", modelMatrix);
        
        // Binding camera position
        material.shader.setUniform("u_CameraPosition", m_sceneState.cameraPosition);

        // Ambient Lighting
        material.shader.setUniform("u_AmbientLightCount", int(m_sceneState.ambientLights.size()));
        for(size_t i = 0; i < m_sceneState.ambientLights.size(); i++) {
            material.shader.setUniform("u_AmbientLights[" + std::to_string(i) + "].color", m_sceneState.ambientLights[i].color);
            material.shader.setUniform("u_AmbientLights[" + std::to_string(i) + "].intensity", m_sceneState.ambientLights[i].intensity);
        }

        // Directional Lights
        material.shader.setUniform("u_DirectionalLightCount", int(m_sceneState.directionalLights.size()));
        for(size_t i = 0; i < m_sceneState.directionalLights.size(); i++) {
            material.shader.setUniform("u_DirectionalLights[" + std::to_string(i) + "].color", m_sceneState.directionalLights[i].color);
            material.shader.setUniform("u_DirectionalLights[" + std::to_string(i) + "].intensity", m_sceneState.directionalLights[i].intensity);
            material.shader.setUniform("u_DirectionalLights[" + std::to_string(i) + "].direction", m_sceneState.directionalLights[i].direction);
        }

        // Point lights
        material.shader.setUniform("u_PointLightCount", int(m_sceneState.pointLights.size()));
        for(size_t i = 0; i < m_sceneState.pointLights.size(); i++) {
            material.shader.setUniform("u_PointLights[" + std::to_string(i) + "].color", m_sceneState.pointLights[i].color);
            material.shader.setUniform("u_PointLights[" + std::to_string(i) + "].intensity", m_sceneState.pointLights[i].intensity);
            material.shader.setUniform("u_PointLights[" + std::to_string(i) + "].position", m_sceneState.pointLights[i].position);
        }

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
    }
}