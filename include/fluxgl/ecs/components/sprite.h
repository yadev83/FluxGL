#pragma once

#include <fluxgl/graphics/texture.h>
#include <fluxgl/graphics/shader.h>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    struct Sprite {
        // static constexpr const char* TypeName = "Sprite";
        // static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
        //     Sprite sprite;
        //     registry.addComponent<Sprite>(entityID, sprite);
        // }

        Texture texture;
        Shader shader;

        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        glm::vec2 size = {1.0f, 1.0f};

        int layer = 0;
        glm::vec2 uvMin = {0.0f, 0.0f};
        glm::vec2 uvMax = {1.0f, 1.0f};
    };
}