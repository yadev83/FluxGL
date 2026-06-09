#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    struct Transform {
        static constexpr const char* TypeName = "Transform";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            Transform transform;
            
            // Parse the json data if it is a valid object, and go through its members to compute the transform component
            if(data.isObject()) {
                Json::Value positionData = data.get("position", Json::objectValue);
                transform.position = {positionData.get("x", 0.0f), positionData.get("y", 0.0f), positionData.get("z", 0.0f)};

                Json::Value rotationData = data.get("rotation", Json::objectValue);
                transform.rotation = {rotationData.get("x", 0.0f), rotationData.get("y", 0.0f), rotationData.get("z", 0.0f)};

                Json::Value scaleData = data.get("scale", Json::objectValue);
                transform.scale = {scaleData.get("x", 1.0f), scaleData.get("y", 1.0f), scaleData.get("z", 1.0f)};
            }

            registry.addComponent<Transform>(entityID, transform);
        }
        
        glm::vec3 position  = glm::vec3(0.0f);
        glm::vec3 rotation  = glm::vec3(0.0f);
        glm::vec3 scale     = glm::vec3(1.0f);

        glm::mat4 getModelMatrix() const {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 r = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
            glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

            return t * r * s;
        }

        glm::vec3 front() const {
            float yaw   = glm::radians(rotation.y - 90.0f);
            float pitch = glm::radians(rotation.x);
            return glm::normalize(glm::vec3(
                cos(yaw) * cos(pitch),
                sin(pitch),
                sin(yaw) * cos(pitch)
            ));
        }

        glm::vec3 right() const {
            return glm::normalize(glm::cross(front(), glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        glm::vec3 up() const {
            return glm::normalize(glm::cross(right(), front()));
        }

        glm::vec3 worldUp() const {
            return glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::vec3 worldRight() const {
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }

        glm::vec3 worldFront() const {
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }
    };
}