#include <vector>
#include <fluxgl/core/app.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/core/log.h>

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/systems/render_system.h>

#include <fluxgl/ecs/components/transform.h>
#include <fluxgl/ecs/components/camera.h>
#include <fluxgl/ecs/components/mesh_renderer.h>
#include <fluxgl/ecs/components/light.h>
#include <fluxgl/ecs/components/ui_transform.h>
#include <fluxgl/ecs/components/ui_rect.h>
#include <fluxgl/ecs/components/ui_text.h>

#include <fluxgl/ui/font.h>
#include <fluxgl/graphics/renderer.h>
#include <fluxgl/graphics/debug_renderer.h>

namespace fluxgl {
    void RenderSystem::onUpdate(fluxgl::Scene& scene, float dt) {
        auto& registry = scene.getRegistry();
        auto& resources = scene.getContext().resourceManager;

        // Clear/Setup frame
        Renderer::beginFrame();

        // Setup lights
        for(auto entity : registry.query<Light>()) {
            auto& light = entity.getComponent<Light>();

            switch(light.type) {
                case LightType::Directional: {
                    if(entity.hasComponent<Transform>()) {
                        auto& transform = entity.getComponent<Transform>();
                        Renderer::registerDirectionalLight(light.color, light.intensity, transform.front());
                    } 
                } break;
                case LightType::Point: {
                    if(entity.hasComponent<Transform>()) {
                        auto& transform = entity.getComponent<Transform>();
                        Renderer::registerPointLight(light.color, light.intensity, transform.position);
                    }
                } break;
                case LightType::Ambient:
                    Renderer::registerAmbientLight(light.color, light.intensity);
                    break;
            }
        }

        // Find the active camera
        Camera* camera = nullptr;
        Transform* cameraTransform = nullptr;
        
        for(auto entity : registry.query<Camera, Transform>()) {
            camera = &entity.getComponent<Camera>();
            cameraTransform = &entity.getComponent<Transform>();
            break;
        }

        // Set the camera
        if(camera && cameraTransform) {
            glm::mat4 viewMatrix = camera->getViewMatrix(*cameraTransform);
            glm::mat4 projectionMatrix = camera->getProjectionMatrix();
            Renderer::setCamera(viewMatrix, projectionMatrix, cameraTransform->position);
            if(DebugRenderer::isEnabled()) DebugRenderer::setCamera(viewMatrix, projectionMatrix);
        }

        // Draw sprites
        for(auto entity : registry.query<Sprite, Transform>()) {
            auto& sprite = entity.getComponent<Sprite>();
            auto& transform = entity.getComponent<Transform>();

            auto shader = resources.getResource<Shader>(sprite.shader);
            auto texture = resources.getResource<Texture>(sprite.texture);

            Renderer::drawSprite(
                transform.getModelMatrix(),
                
                shader,
                texture,
                sprite.layer,
                sprite.color,
                sprite.size,
                sprite.uvMin,
                sprite.uvMax
            );
        }

        // Draw meshes
        for(auto entity : registry.query<MeshRenderer, Transform>()) {
            auto& meshRenderer = entity.getComponent<MeshRenderer>();
            auto& transform = entity.getComponent<Transform>();
            
            auto mesh = resources.getResource<Mesh>(meshRenderer.mesh);
            auto shader = resources.getResource<Shader>(meshRenderer.material.shader);
            auto albedoTextures = resources.getResources<Texture>(meshRenderer.material.albedoTextures);
            auto normalMap = resources.getResource<Texture>(meshRenderer.material.normalMap);
            auto specularMap = resources.getResource<Texture>(meshRenderer.material.specularMap);
            auto emissionMap = resources.getResource<Texture>(meshRenderer.material.emissionMap);

            Renderer::drawMesh(
                mesh,
                transform.getModelMatrix(),
                
                shader,
                albedoTextures,
                normalMap,
                specularMap,
                emissionMap,

                meshRenderer.material.albedoColor,
                meshRenderer.material.specularColor,
                meshRenderer.material.emissionColor,
                meshRenderer.material.shininess
            );
        }

        // Debug renderer (if enabled)
        if(DebugRenderer::isEnabled()) DebugRenderer::flush();

        
        // Draw UI
        Renderer::beginUIPass();
        for(auto entity : registry.query<UITransform, UIRect>()) {
            auto& transform = entity.getComponent<UITransform>();
            auto& image = entity.getComponent<UIRect>();

            auto shader = resources.getResource<Shader>(image.shader);
            auto texture = resources.getResource<Texture>(image.texture);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(
                model,
                glm::vec3(transform.position, 0.0f)
            );

            Renderer::drawUIQuad(
                model,

                shader,
                texture,

                transform.layer,

                image.color,
                transform.scale,

                image.uvMin,
                image.uvMax
            );
        }

        for(auto entity : registry.query<UITransform, UIText>()) {
            auto& transform = entity.getComponent<UITransform>();
            auto& text = entity.getComponent<UIText>();

            auto font = resources.getResource<Font>(text.font);
            auto shader = resources.getResource<Shader>(text.shader);

            if(!font) {
                throw Error{ErrorCode::Error, "Error during RenderSystem::onUpdate: invalid font provided for UIText: " + text.font};
            }

            float cursorX = transform.position.x;

            for(char c : text.text) {
                const Glyph* glyph = font->getGlyph(c);
                if(!glyph) {
                    FLUXGL_LOG_WARNING("Font Glyph not found + " + c);
                    continue;
                }

                float glyphScale = (text.fontSize / glyph->sourceSize);
                float scaleX = transform.scale.x * glyphScale;
                float scaleY = transform.scale.y * glyphScale;

                float x = cursorX + glyph->bearingX * scaleX;
                float y = transform.position.y + glyph->bearingY * scaleY;

                glm::mat4 model(1.0f);
                model = glm::translate(
                    model,
                    glm::vec3(x, y, 0)
                );

                Renderer::drawUIQuad(
                    model,
                    shader,
                    font->getTexture(),
                    transform.layer,
                    text.color,
                    {
                        glyph->width * scaleX,
                        glyph->height * scaleY
                    },
                    {
                        glyph->u0,
                        glyph->v0
                    },
                    {
                        glyph->u1,
                        glyph->v1
                    }
                );

                cursorX += glyph->advance * scaleX;
            }
        }
        Renderer::endUIPass();
    }
}