#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

namespace fluxgl {
    class Entity;
    class Behavior;
    
    using EntityID = unsigned int;
    using ComponentStorage = std::unordered_map<EntityID, void*>;
    using BehaviorStorage = std::vector<Behavior*>;

    class Registry {
        private:
            EntityID m_nextID = 0;
            // Map a type index to a componentStorage
            std::unordered_map<std::type_index, ComponentStorage> m_storages;

            // Map entities to behaviors
            std::unordered_map<EntityID, BehaviorStorage> m_behaviors;

        public:
            Entity createEntity();
            void destroyEntity(EntityID id);
            Entity getEntity(EntityID id);

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

            // Behaviors
            template<typename BehaviorT, typename... Args>
            BehaviorT& registerBehavior(EntityID id, Args&&... args);

            BehaviorStorage getAllBehaviors();
            void clearBehaviors(EntityID id);
    };
}

#include "registry.inl"