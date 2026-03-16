#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

#include <algorithm>

namespace fluxgl {
    void Registry::destroyEntities() {
        for(EntityID id : m_entitiesToDelete) {
            for(auto& [type, storage] : m_storages) {
                if(storage.find(id) == storage.end()) continue;
                storage.erase(id);
            }
            
            m_behaviors.erase(id);
            m_tags.erase(id);
        }

        m_entitiesToDelete.clear();
    }

    void Registry::update(float dt) {
        destroyEntities();
    }

    // Entities
    Entity Registry::createEntity() {
        return Entity(m_nextID++, this);
    }

    void Registry::destroyEntity(EntityID id) {
        m_entitiesToDelete.push_back(id);
    }

    bool Registry::isValidEntity(EntityID id) {
        // Make sure that entity has at least one component storage, and is not marked for deletion
        if(std::find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), id) != m_entitiesToDelete.end()) return false;

        // Otherwise, isValid (not true really, but it works for now)
        // TODO : Store valid ids inside the registry and check on that instead
        return true;
    }

    Entity Registry::getEntity(EntityID id) {
        return Entity(id, this);
    }

    // Tags
    void Registry::addTag(EntityID id, const std::string& tag) {
        if(!hasTag(id, tag)) {
            auto& storage = m_tags[id];
            storage.push_back(tag);
        }
    }

    bool Registry::hasTag(EntityID id, const std::string& tag) {
        auto it = m_tags.find(id);
        if(it == m_tags.end()) return false;
        auto& storage = it->second;
        
        return std::find(storage.begin(), storage.end(), tag) != storage.end();
    }

    void Registry::removeTag(EntityID id, const std::string& tag) {
        if(hasTag(id, tag)) {
            auto& storage = m_tags[id];
            storage.erase(std::remove(storage.begin(), storage.end(), tag), storage.end());
        }
    }

    // Behaviors
    BehaviorStorage Registry::getAllBehaviors() {
        BehaviorStorage output;

        for(auto& [id, behaviors] : m_behaviors) {
            output.insert(output.end(), behaviors.begin(), behaviors.end());
        }

        return output;
    }
}