#pragma once

#include <fluxgl/graphics/material.h>
#include <fluxgl/graphics/mesh.h>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>


namespace fluxgl {
    struct MeshRenderer {
        // static constexpr const char* TypeName = "MeshRenderer";
        // static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
        //     MeshRenderer renderer;
        //     registry.addComponent<MeshRenderer>(entityID, renderer);
        // }

        Material material;
        Mesh mesh;
    };
}