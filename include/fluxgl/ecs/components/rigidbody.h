#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    enum class RigidBodyType {
        Static, // No move, no physics (walls, grounds, etc...)
        Dynamic, // Moves and is handled by the physics system, gravity and collisions, can be pushed ?
        Kinematic // No gravity, but can be moved by the code and has collisions as well
    };

    struct RigidBody2D {
        static constexpr const char* TypeName = "fluxgl::RigidBody2D";
        static void Create(Registry& registry, EntityID entityID, const Json::Value& data) {
            RigidBody2D rigidBody2D;

            if(data.isObject()) {
                Json::Value typeData = data.get("type", "dynamic");
                if(typeData.asString() == "static") rigidBody2D.type = RigidBodyType::Static;
                if(typeData.asString() == "dynamic") rigidBody2D.type = RigidBodyType::Dynamic;
                if(typeData.asString() == "kinematic") rigidBody2D.type = RigidBodyType::Kinematic;

                rigidBody2D.gravityScale = data.get("gravityScale", 1.0f).asFloat();
            }

            registry.addComponent<RigidBody2D>(entityID, rigidBody2D);
        }

        RigidBodyType type = RigidBodyType::Dynamic;
        glm::vec2 velocity = {0.0f, 0.0f};
        glm::vec2 direction = {0.0f, 0.0f}; // Automatically updated by the physics system, normalized vector of the velocity

        float gravityScale = 1.0f;
        bool grounded = false;
    };
}