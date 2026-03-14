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
        auto& storage = m_tags[id];
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