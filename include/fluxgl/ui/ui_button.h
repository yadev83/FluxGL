#pragma once

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

#include <json/json.h>
#include <glm/glm.hpp>

namespace fluxgl {
    struct UIButton {
        static constexpr const char* TypeName = "fluxgl::UIButton";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            registry.addComponent<UIButton>(entityID);
        }

        bool hovered = false;
        bool pressed = false;
    };
}