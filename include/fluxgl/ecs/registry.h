#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>
#include <string>

namespace fluxgl {
    class Entity;
    class Behavior;
    
    using EntityID = unsigned int;
    using ComponentStorage = std::unordered_map<EntityID, void*>;
    using BehaviorStorage = std::vector<Behavior*>;
    using TagsStorage = std::vector<std::string>;

    class Registry {
        private:
            EntityID m_nextID = 0;
            // Map a type index to a componentStorage
            std::unordered_map<std::type_index, ComponentStorage> m_storages;
            // Map entities to behaviors
            std::unordered_map<EntityID, BehaviorStorage> m_behaviors;
            // Map entities to tags
            std::unordered_map<EntityID, TagsStorage> m_tags;

            // Entities to delete on next frame
            std::vector<EntityID> m_entitiesToDelete;
            void destroyEntities();

        public:
            // Registry frame mgt
            void update(float dt);

            // Entities management
            Entity createEntity();
            void destroyEntity(EntityID id); // Marks entity for deletion
            Entity getEntity(EntityID id);

            // Entity components
            template<typename ComponentT, typename... Args>
            ComponentT& addComponent(EntityID id, Args&&... args);

            template<typename ComponentT>
            ComponentT& getComponent(EntityID id);

            template<typename ComponentT>
            bool hasComponent(EntityID id);

            template<typename ComponentT>
            void removeComponent(EntityID id);

            template<typename FirstT, typename... RestT>
            std::vector<Entity> query();

            // Entity Tags
            void addTag(EntityID id, const std::string& tag);
            bool hasTag(EntityID id, const std::string& tag);
            void removeTag(EntityID id, const std::string& tag);

            // Entity Behaviors
            template<typename BehaviorT, typename... Args>
            BehaviorT& registerBehavior(EntityID id, Args&&... args);
            
            BehaviorStorage getAllBehaviors();
    };
}

#include "registry.inl"