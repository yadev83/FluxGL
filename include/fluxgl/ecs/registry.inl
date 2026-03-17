#include <stdexcept>

namespace fluxgl {
    template<typename ComponentT, typename... Args>
    ComponentT& Registry::addComponent(EntityID id, Args&&... args) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::AddComponent" + std::string(typeid(ComponentT).name()) + ": Invalid entity" + std::to_string(id));

        auto& storage = m_storages[typeid(ComponentT)];

        if(storage.find(id) == storage.end()) {
            storage[id] = new ComponentT(std::forward<Args>(args)...);
        }

        return *static_cast<ComponentT*>(storage[id]);
    }

    template<typename ComponentT>
    ComponentT& Registry::getComponent(EntityID id) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::GetComponent" + std::string(typeid(ComponentT).name()) + ": Invalid entity" + std::to_string(id));
        if(!hasComponent<ComponentT>(id)) throw std::runtime_error("Registry::GetComponent: Component not found");

        auto& storage = m_storages[typeid(ComponentT)];
        return *static_cast<ComponentT*>(storage[id]);
    }

    template<typename ComponentT>
    bool Registry::hasComponent(EntityID id) {
        if(!isValidEntity(id)) return false;

        auto& storage = m_storages[typeid(ComponentT)];
        return storage.find(id) != storage.end();
    }

    template<typename ComponentT>
    void Registry::removeComponent(EntityID id) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::RemoveComponent" + std::string(typeid(ComponentT).name()) + ": Invalid entity" + std::to_string(id));
        if(!hasComponent<ComponentT>(id)) throw std::runtime_error("Registry::RemoveComponent: Component" + std::string(typeid(ComponentT).name()) + " not found for entity " + std::to_string(id));

        auto& storage = m_storages[typeid(ComponentT)];
        storage.erase(id);
    }

    template<typename FirstT, typename... RestT>
    std::vector<Entity> Registry::query() {
        std::vector<Entity> result;

        for(auto& [type, storage] : m_storages) {
            for(auto& [entityID, compPtr] : storage) {
                if(!isAliveEntity(entityID)) continue; // Skip invalid entities for queries
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