#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <sstream>

#define FLUXGL_MAX_ENTITIES 10000

namespace fluxgl {
    class Entity;
    struct EntityState;
    class Behavior;
    
    using EntityID = unsigned int;
    using ComponentStorage = std::unordered_map<EntityID, void*>;
    using BehaviorStorage = std::vector<Behavior*>;
    using TagsStorage = std::vector<std::string>;

    class Registry {
        private:
            std::vector<EntityID> m_availableIDs;

            // Map a type index to a componentStorage
            std::unordered_map<std::type_index, ComponentStorage> m_storages;
            std::vector<std::type_index> getEntityComponentTypes(EntityID id); // Utils to grab entity component names
            // Map entities to behaviors
            std::unordered_map<EntityID, BehaviorStorage> m_behaviors;
            // Map entities to tags
            std::unordered_map<EntityID, TagsStorage> m_tags;
            // Map entities to parent entities (hierarchy)
            std::unordered_map<EntityID, EntityID> m_hierarchy; // (m_hierarchy[child] = parent;)
            // Map entities to state
            std::unordered_map<EntityID, EntityState> m_states;

            // Entities to delete on next frame
            std::set<EntityID> m_entitiesToDelete;
            void removeEntity(EntityID id); // Remove entity from the registry
            void removeEntitiesMarkedForDestruction();

        public:
            Registry();
            void dumpEntity(std::stringstream& ss, EntityID id, int depth = 0);
            std::string toString();

            // Registry frame mgt
            void update(float dt);
            void clear(); // Clear all entities, components and behaviors

            // Entities management
            Entity createEntity();
            void destroyEntity(EntityID id); // Marks entity for deletion
            bool isValidEntity(EntityID id);
            bool isAliveEntity(EntityID id);
            bool isEntityInUse(EntityID id);
            bool isEntityEnabled(EntityID id);
            Entity getEntity(EntityID id);
            void setParent(EntityID child, EntityID parent = 0);
            void setEntityEnabled(EntityID id, bool enabled);
            void removeChild(EntityID child);
            std::vector<Entity> getChildren(EntityID id);
            Entity getParent(EntityID id);

            // Entity components
            template<typename ComponentT, typename... Args>
            ComponentT& addComponent(EntityID id, Args&&... args);

            template<typename ComponentT>
            ComponentT& getComponent(EntityID id);

            template<typename ComponentT>
            bool hasComponent(EntityID id);

            template<typename ComponentT>
            void removeComponent(EntityID id);

            // Entity Query
            template<typename FirstT, typename... RestT>
            std::vector<Entity> query();

            std::vector<Entity> queryByTag(const std::string& tag);

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