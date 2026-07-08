#include <fluxgl/graphics/renderer.h>
#include <fluxgl/core/log.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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
        } else {
            m_state.viewportWidth = width;
            m_state.viewportHeight = height;
            m_state.viewportX = 0.0f;
            m_state.viewportY = 0.0f;
        }
    }

    float Renderer::getViewportAspectRatio() {
        return (float)m_state.viewportWidth / m_state.viewportHeight;
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

    void Renderer::beginUIPass() {
        glEnable(GL_BLEND);

        glBlendFunc(
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA
        );
    }

    void Renderer::endUIPass() {
        glDisable(GL_BLEND);
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

    void Renderer::drawSprite(
        const glm::mat4& modelMatrix,

        const Shader* shader,
        const Texture* texture,
        const int layer,
        const glm::vec3& color,
        const glm::vec2& size,
            const glm::vec2& uvMin,
        const glm::vec2& uvMax
    ) {
        if(!(shader && shader->isValid())) throw std::runtime_error("Invalid shader provided to Renderer::drawSprite");
        shader->bind();

        if(texture && texture->isValid()) {
            texture->bind(0);
            shader->setUniform("u_Texture", 0);
            shader->setUniform("u_UseTexture", true);
        } else {
            shader->setUniform("u_UseTexture", false);
        }

        shader->setUniform("u_Color", color);
        shader->setUniform("u_UVMin", uvMin);
        shader->setUniform("u_UVMax", uvMax);

        glm::mat4 model = modelMatrix;
        model = glm::scale(model, glm::vec3(size, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, layer));

        shader->setUniform("u_View", m_sceneState.viewMatrix);
        shader->setUniform("u_Projection", m_sceneState.projectionMatrix);
        shader->setUniform("u_Model", model);

        static Mesh quad = Mesh::quad();
        unsigned int vao = quad.getVAO();
        size_t indexCount = quad.getIndexCount();
        size_t verticesCount = quad.getVerticesCount();
        if (vao > 0) { 
            glBindVertexArray(vao); 
            if (indexCount > 0) { 
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); 
            } else { 
                glDrawArrays(GL_TRIANGLES, 0, verticesCount); 
            } 
        }
    }

    void Renderer::drawUIQuad(
        const glm::mat4& modelMatrix,
        
        const Shader* shader,
        const Texture* texture,
        const int layer,
        const glm::vec4& color,
        const glm::vec2& size,
        const glm::vec2& uvMin,
        const glm::vec2& uvMax
    ) {
        if(!(shader && shader->isValid())) throw std::runtime_error("Invalid shader provided to Renderer::drawSprite");
        shader->bind();

        if(texture && texture->isValid()) {
            texture->bind(0);
            shader->setUniform("u_Texture", 0);
            shader->setUniform("u_UseTexture", true);
        } else {
            shader->setUniform("u_UseTexture", false);
        }

        shader->setUniform("u_Color", color);
        shader->setUniform("u_UVMin", uvMin);
        shader->setUniform("u_UVMax", uvMax);

        glm::mat4 model = modelMatrix;
        model = glm::scale(model, glm::vec3(size, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, layer));

        shader->setUniform("u_Projection", glm::ortho(
            0.0f,
            (float)(m_state.framebufferWidth),
            (float)(m_state.framebufferHeight),
            0.0f,
            -1000.0f,
            1000.0f
        ));
        shader->setUniform("u_Model", model);

        static Mesh quad = Mesh::quad();
        unsigned int vao = quad.getVAO();
        size_t indexCount = quad.getIndexCount();
        size_t verticesCount = quad.getVerticesCount();
        if (vao > 0) { 
            glBindVertexArray(vao); 
            if (indexCount > 0) { 
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); 
            } else { 
                glDrawArrays(GL_TRIANGLES, 0, verticesCount); 
            } 
        }
    }

    void Renderer::drawMesh(
        const Mesh* mesh,
        const glm::mat4& modelMatrix,
        
        const Shader* shader,
        const std::vector<Texture*> albedoTextures,
        const Texture* normalMap,
        const Texture* specularMap,
        const Texture* emissionMap,

        const glm::vec3& albedoColor,
        const glm::vec3& specularColor,
        const glm::vec3& emissionColor,
        const float shininess
    ) {
        if(!(shader && shader->isValid())) throw std::runtime_error("Invalid shader provided to Renderer::drawMesh");
        shader->bind();

        if(!mesh) throw std::runtime_error("Invalid mesh provided to Renderer::drawMesh");

        // Binding textures/maps
        int slot = 0;
        for(size_t i = 0; i < albedoTextures.size(); i++) {
            if(albedoTextures[i]->isValid()) {
                albedoTextures[i]->bind(slot);
                shader->setUniform("u_Albedo[" + std::to_string(slot) + "]", slot);
                slot++;
            }
        }
        shader->setUniform("u_AlbedoTextureCount", slot);

        if(normalMap && normalMap->isValid()) {
            normalMap->bind(slot);
            shader->setUniform("u_NormalMap", slot);
            shader->setUniform("u_UseNormalMap", true);
            slot++;
        } else {
            shader->setUniform("u_UseNormalMap", false);
        }

        if(specularMap && specularMap->isValid()) {
            specularMap->bind(slot);
            shader->setUniform("u_SpecularMap", slot);
            shader->setUniform("u_UseSpecularMap", true);
            slot++;
        } else {
            shader->setUniform("u_UseSpecularMap", false);
        }

        if(emissionMap && emissionMap->isValid()) {
            emissionMap->bind(slot);
            shader->setUniform("u_EmissionMap", slot);
            shader->setUniform("u_UseEmissionMap", true);
            slot++;
        } else {
            shader->setUniform("u_UseEmissionMap", false);
        }

        // Binding colors
        shader->setUniform("u_AlbedoColor", albedoColor);
        shader->setUniform("u_SpecularColor", specularColor);
        shader->setUniform("u_EmissionColor", emissionColor);
        // Binding material properties
        shader->setUniform("u_SpecularColor", specularColor);
        shader->setUniform("u_Shininess", shininess);

        // Binding MVP matrices
        shader->setUniform("u_View", m_sceneState.viewMatrix);
        shader->setUniform("u_Projection", m_sceneState.projectionMatrix);
        shader->setUniform("u_Model", modelMatrix);
        
        // Binding camera position
        shader->setUniform("u_CameraPosition", m_sceneState.cameraPosition);

        // Ambient Lighting
        shader->setUniform("u_AmbientLightCount", int(m_sceneState.ambientLights.size()));
        for(size_t i = 0; i < m_sceneState.ambientLights.size(); i++) {
            shader->setUniform("u_AmbientLights[" + std::to_string(i) + "].color", m_sceneState.ambientLights[i].color);
            shader->setUniform("u_AmbientLights[" + std::to_string(i) + "].intensity", m_sceneState.ambientLights[i].intensity);
        }

        // Directional Lights
        shader->setUniform("u_DirectionalLightCount", int(m_sceneState.directionalLights.size()));
        for(size_t i = 0; i < m_sceneState.directionalLights.size(); i++) {
            shader->setUniform("u_DirectionalLights[" + std::to_string(i) + "].color", m_sceneState.directionalLights[i].color);
            shader->setUniform("u_DirectionalLights[" + std::to_string(i) + "].intensity", m_sceneState.directionalLights[i].intensity);
            shader->setUniform("u_DirectionalLights[" + std::to_string(i) + "].direction", m_sceneState.directionalLights[i].direction);
        }

        // Point lights
        shader->setUniform("u_PointLightCount", int(m_sceneState.pointLights.size()));
        for(size_t i = 0; i < m_sceneState.pointLights.size(); i++) {
            shader->setUniform("u_PointLights[" + std::to_string(i) + "].color", m_sceneState.pointLights[i].color);
            shader->setUniform("u_PointLights[" + std::to_string(i) + "].intensity", m_sceneState.pointLights[i].intensity);
            shader->setUniform("u_PointLights[" + std::to_string(i) + "].position", m_sceneState.pointLights[i].position);
        }

        // DRAW CALL
        unsigned int vao = mesh->getVAO();
        size_t indexCount = mesh->getIndexCount();
        size_t verticesCount = mesh->getVerticesCount();

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