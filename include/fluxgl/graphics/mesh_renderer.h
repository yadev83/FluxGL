#pragma once

#include <fluxgl/graphics/material.h>
#include <fluxgl/assets/assets.h>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>


namespace fluxgl {
    struct MeshRenderer {
        static constexpr const char* TypeName = "fluxgl::MeshRenderer";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            MeshRenderer renderer;

            if(data.isObject()) {
                Json::Value materialData = data.get("material", Json::objectValue);
                if(materialData.isObject() && materialData.getMemberNames().size()) {
                    renderer.material.shader = materialData.get("shader", "").asString();
                    
                    Json::Value albedoColorData = materialData.get("albedoColor", Json::objectValue);
                    renderer.material.albedoColor = {albedoColorData.get("r", 1.0f).asFloat(), albedoColorData.get("g", 1.0f).asFloat(), albedoColorData.get("b", 1.0f).asFloat()};

                    Json::Value specularColorData = materialData.get("specularColor", Json::objectValue);
                    renderer.material.specularColor = {specularColorData.get("r", 1.0f).asFloat(), specularColorData.get("g", 1.0f).asFloat(), specularColorData.get("b", 1.0f).asFloat()};

                    Json::Value emissionColorData = materialData.get("emissionColor", Json::objectValue);
                    renderer.material.emissionColor = {emissionColorData.get("r", 0.0f).asFloat(), emissionColorData.get("g", 0.0f).asFloat(), emissionColorData.get("b", 0.0f).asFloat()};

                    renderer.material.shininess = materialData.get("shininess", 32.0f).asFloat();

                    Json::Value albedoTexturesData = materialData.get("albedoTextures", Json::arrayValue);
                    for(auto albedoTextureData: albedoTexturesData) {
                        renderer.material.albedoTextures.push_back(albedoTextureData.asString());
                    }

                    renderer.material.normalMap = materialData.get("normalMap", "").asString();
                    renderer.material.specularMap = materialData.get("specularMap", "").asString();
                    renderer.material.emissionMap = materialData.get("emissionMap", "").asString();
                }

                renderer.mesh = data.get("mesh", "").asString();
            }

            registry.addComponent<MeshRenderer>(entityID, renderer);
        }

        Material material;
        Resource mesh;
    };
}