#pragma once

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    enum class FacingDirection {
        Left,
        Right
    };

    struct Facing2D {
        static constexpr const char* TypeName = "fluxgl::Facing2D";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            std::string direction = data.get("direction", "right").asString();
            Facing2D facing(direction == "left" ? FacingDirection::Left : FacingDirection::Right);
            registry.addComponent<Facing2D>(entityID, facing);
        }

        FacingDirection direction = FacingDirection::Right;

        Facing2D(FacingDirection dir = FacingDirection::Right) : direction(dir) {}
    };
}