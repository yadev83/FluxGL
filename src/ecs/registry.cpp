#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    Entity Registry::createEntity() {
        return Entity(m_nextID++, this);
    }

    void Registry::destroyEntity(EntityID id) {
        for(auto& [type, storage] : m_storages) {
            if(storage.find(id) == storage.end()) continue;
            storage.erase(id);
        }

        clearBehaviors(id);
    }

    Entity Registry::getEntity(EntityID id) {
        return Entity(id, this);
    }

    BehaviorStorage Registry::getAllBehaviors() {
        BehaviorStorage output;

        for(auto& [id, behaviors] : m_behaviors) {
            output.insert(output.end(), behaviors.begin(), behaviors.end());
        }

        return output;
    }

    void Registry::clearBehaviors(EntityID id)  {
        m_behaviors[id].clear();
    }
}