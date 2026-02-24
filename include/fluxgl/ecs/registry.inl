#include <stdexcept>

namespace fluxgl {
    template<typename ComponentT, typename... Args>
    ComponentT& Registry::addComponent(EntityID id, Args&&... args) {
        auto& storage = m_storages[typeid(ComponentT)];

        if(storage.find(id) == storage.end()) {
            storage[id] = new ComponentT(std::forward<Args>(args)...);
        }

        return *static_cast<ComponentT*>(storage[id]);
    }

    template<typename ComponentT>
    ComponentT& Registry::getComponent(EntityID id) {
        auto& storage = m_storages[typeid(ComponentT)];
        return *static_cast<ComponentT*>(storage[id]);
    }

    template<typename ComponentT>
    bool Registry::hasComponent(EntityID id) {
        auto& storage = m_storages[typeid(ComponentT)];
        return storage.find(id) != storage.end();
    }

    template<typename ComponentT>
    void Registry::removeComponent(EntityID id) {
        auto& storage = m_storages[typeid(ComponentT)];
        storage.erase(id);
    }

    template<typename FirstT, typename... RestT>
    std::vector<Entity> Registry::query() {
        std::vector<Entity> result;

        for(auto& [type, storage] : m_storages) {
            for(auto& [entityID, compPtr] : storage) {
                if(hasComponent<FirstT>(entityID) && (hasComponent<RestT>(entityID) && ...)) {
                    result.push_back(Entity(entityID, this));                         
                }
            }
        }
        
        return result;
    }

    // Behaviors
    template<typename BehaviorT, typename... Args>
    BehaviorT& Registry::registerBehavior(EntityID id, Args&&... args) {
        if(!std::is_base_of<Behavior, BehaviorT>::value) {
            throw std::runtime_error("BehaviorT must inherit from Behavior");
        }

        auto& storage = m_behaviors[id];
        storage.emplace_back(new BehaviorT(std::forward<Args>(args)...));

        BehaviorT* behavior = static_cast<BehaviorT*>(storage.back());
        behavior->attachEntity(id);

        return *static_cast<BehaviorT*>(storage.back());
    }
}