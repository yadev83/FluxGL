#include <fluxgl/assets/prefab_loader.h>

#include <fstream>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    Prefab::Prefab() {}

    Prefab::Prefab(std::string path) {
        loadFromFile(path);
    }

    void Prefab::loadFromFile(std::string path) {
        std::ifstream file(path, std::ifstream::binary);
        file >> m_data;
    }

    Json::Value& Prefab::getData() {
        return m_data;
    }

    Json::Value PrefabLoader::interpretValue(const Json::Value& value, Registry* registry, EntityID entityID, EntityID parentID) {
        if(value.isObject()) {
            Json::Value result(Json::objectValue);

            for(const auto& key : value.getMemberNames()) {
                result[key] = interpretValue(value[key], registry, entityID, parentID);
            }

            return result;
        }

        if(value.isArray()) {
            Json::Value result(Json::arrayValue);

            for(Json::ArrayIndex i = 0; i < value.size(); ++i) {
                result.append(interpretValue(value[i], registry, entityID, parentID));
            }

            return result;
        }
        
        if(value.isString()) {
            std::string str = value.asString();
            if(!str.empty() && str[0] == '@') {
                if(str == "@parent") return Json::Value(static_cast<int>(parentID));
                if(str == "@self") return Json::Value(static_cast<int>(entityID));

                FLUXGL_LOG_WARNING("Prefab special data value " + value.asString() + " is invalid");
                return value;
            }
        }

        return value;
    }

    Entity PrefabLoader::instantiate(Registry* registry, Prefab& prefab) {
        /**
         * Prefab json format is as follows :
         * 
         * {
         *      "name": "prefabName",
         *      "components": [
         *          {
         *              "type": "componentType",
         *              "data" (optional): {
         *                  "key": "value"
         *              }
         *          } 
         *      ],
         *      "behaviors": [
         *          {
         *              "type": "behaviorType",
         *              "data" (optional): {
         *                  "key": "value" 
         *              }
         *          }
         *      ],
         *      "children": [
         *          {...},
         *          {Same content but for eventual children entities},
         *          {}
         *      ]
         * }
         */

        // Create the entity
        Entity entity = registry->createEntity();
        Json::Value& data = prefab.getData();

        // Load the prefab
        loadPrefab(data, registry, entity.getID());

        return entity;
    }

    Entity PrefabLoader::instantiate(Registry* registry, const std::string& path) {
        Prefab prefab(path);
        return instantiate(registry, prefab);
    }

    void PrefabLoader::loadPrefab(const Json::Value& prefab, Registry* registry, EntityID entityID, EntityID parentID) {
        // Read the prefab from the JSON::value (empty string if tags aren't set)
        Json::Value prefabTags = prefab.get("tags", Json::arrayValue);
        if(prefabTags.size()) {
            for(int i = 0; i < prefabTags.size(); i++) {
                std::string tag = prefabTags.get(i, "").asString();
                registry->getEntity(entityID).addTag(tag);
            }
        }

        // Read the components from the JSON::value
        Json::Value components = prefab.get("components", Json::arrayValue);
        if(components.size()) loadPrefabComponents(components, registry, entityID, parentID);

        // Read the behaviors from the JSON value
        Json::Value behaviors = prefab.get("behaviors", Json::arrayValue);
        if(behaviors.size()) loadPrefabBehaviors(behaviors, registry, entityID, parentID);

        // Children
        Json::Value children = prefab.get("children", Json::arrayValue);
        if(children.size()) {
            // Iterate for each child
            for(int i = 0; i < children.size(); i++) {
                Json::Value childPrefab = children[i];           
                Entity childEntity = registry->getEntity(entityID).appendChild();
                loadPrefab(childPrefab, registry, childEntity.getID(), entityID); // Load prefab with current entity as parent for the child
            }
        }
    }

    void PrefabLoader::loadPrefabComponents(const Json::Value& components, Registry* registry, EntityID entityID, EntityID parentID) {
        for (int i = 0; i < components.size(); i++) {
            Json::Value component = components[i];

            std::string componentType = component["type"].asString();
            if(componentType.empty()) {
                FLUXGL_LOG_WARNING("Prefab component at index " + std::to_string(i) + " is missing a 'type' field. Skipping this component.");
                continue;
            }

            auto it = m_componentFactories.find(componentType);
            if(it == m_componentFactories.end()) {
                FLUXGL_LOG_WARNING("Prefab component at index " + std::to_string(i) + " has an unkown 'type'. Skipping this component.");
                continue;
            }

            Json::Value componentData = interpretValue(component.get("data", Json::Value()), registry, entityID, parentID);
            if(it != m_componentFactories.end()) {
                ComponentFactory factory = it->second;
                factory(*registry, entityID, componentData);
            }
        }
    }

    void PrefabLoader::loadPrefabBehaviors(const Json::Value& behaviors, Registry* registry, EntityID entityID, EntityID parentID) {
        for (int i = 0; i < behaviors.size(); i++) {
            Json::Value behavior = behaviors[i];
            
            std::string behaviorType = behavior["type"].asString();
            if(behaviorType.empty()) {
                FLUXGL_LOG_WARNING("Prefab behavior at index " + std::to_string(i) + " is missing a 'type' field. Skipping this behavior.");
                continue;
            }

            auto it = m_behaviorFactories.find(behaviorType);
            if(it == m_behaviorFactories.end()) {
                FLUXGL_LOG_WARNING("Prefab behavior at index " + std::to_string(i) + " has an unknown 'type' field. Skipping this behavior.");
                continue;
            }

            Json::Value behaviorData = interpretValue(behavior.get("data", Json::Value()), registry, entityID, parentID);
            if(it != m_behaviorFactories.end()) {
                BehaviorFactory factory = it->second;
                factory(*registry, entityID, behaviorData);
            }
        }
    }
}