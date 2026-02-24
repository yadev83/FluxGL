#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    Entity::Entity(EntityID id, Registry* registry) : m_id(id), m_registry(registry) {}

    EntityID Entity::getID() const {
        return m_id; 
    }

    void Entity::clearBehaviors() {
        m_registry->clearBehaviors(m_id);
    }
}