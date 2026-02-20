#include "registry.h"

namespace fluxgl {
    template<typename ComponentT, typename... Args>
    ComponentT& Entity::addComponent(Args&&... args) {
        return m_registry->addComponent<ComponentT>(m_id, std::forward<Args>(args)...);
    }

    template<typename ComponentT>
    ComponentT& Entity::getComponent() {
        return m_registry->getComponent<ComponentT>(m_id);
    }

    template<typename ComponentT>
    bool Entity::hasComponent() {
        return m_registry->hasComponent<ComponentT>(m_id);
    }

    template<typename ComponentT>
    void Entity::removeComponent() {
        m_registry->removeComponent<ComponentT>(m_id);
    }

    template<typename BehaviorT, typename... Args>
    BehaviorT& Entity::registerBehavior(Args&&... args) {
        return m_registry->registerBehavior<BehaviorT>(m_id, std::forward<Args>(args)...);
    }
}