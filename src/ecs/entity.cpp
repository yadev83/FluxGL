#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    Entity::Entity(EntityID id, Registry* registry) : m_id(id), m_registry(registry) {}

    bool Entity::isValid() {
        return m_registry->isValidEntity(m_id);
    }

    void Entity::destroy() {
        m_registry->destroyEntity(m_id);
    }

    EntityID Entity::getID() const {
        return m_id; 
    }

    void Entity::addTag(const std::string& tag) {
        m_registry->addTag(m_id, tag);
    }

    bool Entity::hasTag(const std::string& tag) {
        return m_registry->hasTag(m_id, tag);
    }

    void Entity::removeTag(const std::string& tag) {
        m_registry->removeTag(m_id, tag);
    }
}