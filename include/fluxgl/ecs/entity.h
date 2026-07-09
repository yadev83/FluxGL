#pragma once

#include <string>
#include <vector>

namespace fluxgl {
    using EntityID = unsigned int;
    class Registry;

    struct EntityState {
        bool enabled = true;
    };

    class Entity {
        private:
            EntityID m_id;
            Registry* m_registry;

        public:
            Entity() = default;
            Entity(EntityID id, Registry* registry);
            bool isValid();
            Entity appendChild();
            Entity getParent();
            std::vector<Entity> getChildren();
            void destroy();
            void disable();
            void enable();
            bool isEnabled();

            EntityID getID() const;
            void addTag(const std::string& tag);
            bool hasTag(const std::string& tag);
            void removeTag(const std::string& tag);

            template<typename ComponentT, typename... Args>
            ComponentT& addComponent(Args&&... args);

            template<typename ComponentT>
            ComponentT& getComponent();

            template<typename ComponentT>
            bool hasComponent();

            template<typename ComponentT>
            void removeComponent();

            template<typename BehaviorT, typename... Args>
            BehaviorT& registerBehavior(Args&&... args);
    };
}

#include "entity.inl"