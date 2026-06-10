#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    using ComponentFactory = std::function<void(
        Registry&,
        EntityID,
        const Json::Value&
    )>;

    using BehaviorFactory = std::function<void(
        Registry&,
        EntityID,
        const Json::Value&
    )>;

    class Prefab {
        private:
            Json::Value m_data;
        
        public:
            Prefab();
            Prefab(std::string path);
            void loadFromFile(std::string path);

            Json::Value& getData();
    };

    class PrefabLoader {
        private:
            std::unordered_map<std::string, ComponentFactory> m_componentFactories;
            std::unordered_map<std::string, BehaviorFactory> m_behaviorFactories;

            void loadPrefab(const Json::Value& prefabData, Registry* registry, EntityID entityID, EntityID parentID = 0);
            void loadPrefabComponents(const Json::Value& components, Registry* registry, EntityID entityID, EntityID parentID = 0);
            void loadPrefabBehaviors(const Json::Value& behaviors, Registry* registry, EntityID entityID, EntityID parentID = 0);

        public:
            Entity instantiate(Registry* registry, Prefab& prefab);
            Entity instantiate(Registry* registry, const std::string& path);

            template <typename T>
            void registerComponent() {
                m_componentFactories[T::TypeName] = &T::Create;
            }

            template <typename T>
            void registerBehavior() {
                m_behaviorFactories[T::TypeName] = &T::Create;
            }

            static Json::Value interpretValue(const Json::Value& value, Registry* registry = nullptr, EntityID entityID = 0, EntityID parentID = 0);
    };
}