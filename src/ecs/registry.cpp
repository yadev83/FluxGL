#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/behavior.h>
#include <fluxgl/core/log.h>

#include <algorithm>

namespace fluxgl {
    Registry::Registry() {
        FLUXGL_LOG_DEBUG("Initializing registry...");
        for(int i = (FLUXGL_MAX_ENTITIES); i > 0; i--) {
            m_availableIDs.push_back(i);
            if(i % 100 == 0) FLUXGL_LOG_DEBUG("Available entities: " + std::to_string(m_availableIDs.size()));
        }
        FLUXGL_LOG_DEBUG("Available entities: " + std::to_string(m_availableIDs.size()));
    }

    std::vector<std::type_index> Registry::getEntityComponentTypes(EntityID id) {
        std::vector<std::type_index> result;
        
        for(auto& [type, storage] : m_storages) {
            if(storage.contains(id)) {
                result.push_back(type);
            }
        }

        return result;
    }

    void Registry::dumpEntity(std::stringstream& ss, EntityID id, int depth) {
        std::string indent(depth * 2, ' ');

        ss << indent << "Entity " << id << '\n';

        if(auto tagIt = m_tags.find(id); tagIt != m_tags.end()) {
            ss << indent << "   Tags: ";

            for(auto& tag : tagIt->second) ss << tag << "; ";
            ss << '\n';
        }

        ss << indent << "   Components: " << '\n';
        for(auto& type : getEntityComponentTypes(id)) {
            ss << indent << "       " << type.name() << '\n';
        }

        if(m_behaviors.find(id) != m_behaviors.end()) {
            ss << indent << "   Behaviors: " << '\n';
            for(Behavior* behavior : m_behaviors[id]) {
                ss << indent << "       " << typeid(*behavior).name() << '\n';
            }
        }
        
        for(Entity child : getChildren(id)) dumpEntity(ss, child.getID(), depth + 1);
    }

    std::string Registry::toString() {
        std::stringstream ss;

        ss << "Registry Dump\n";

        for(EntityID id = 1; id < FLUXGL_MAX_ENTITIES; ++id) {
            if(!isValidEntity(id)) continue;
            if(!isEntityInUse(id)) continue;
            if(m_hierarchy.contains(id)) continue;

            dumpEntity(ss, id);
        }

        return ss.str();
    }

    void Registry::removeEntity(EntityID id) {
        FLUXGL_LOG_TRACE("Registry::removeEntity: " + std::to_string(id));

        // Before doing anything, go through the hirearchy, and remove every children to this entity
        for(auto& child : getChildren(id)) {
            removeEntity(child.getID());
        }

        // Delete every storage / component associated with this id
        for(auto& [type, storage] : m_storages) {
            if(storage.find(id) == storage.end()) continue;
            storage.erase(id);
        }
        
        // Remove behaviors and tags
        m_behaviors.erase(id);
        m_tags.erase(id);

        // Make the id available again for reuse
        m_availableIDs.push_back(id);
    }

    void Registry::removeEntitiesMarkedForDestruction() {
        for(EntityID id : m_entitiesToDelete) removeEntity(id);
        m_entitiesToDelete.clear();
    }

    void Registry::update(float dt) {
        removeEntitiesMarkedForDestruction();
    }

    // Entities
    Entity Registry::createEntity() {
        if(m_availableIDs.empty()) throw std::runtime_error("Registry::CreateEntity: No available entities");

        EntityID id = m_availableIDs.back();
        m_availableIDs.pop_back();

        FLUXGL_LOG_TRACE("Registry::createEntity: " + std::to_string(id));
        return Entity(id, this);
    }

    void Registry::destroyEntity(EntityID id) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::DestroyEntity: Invalid entity" + std::to_string(id));
        
        if(!isAliveEntity(id)) return;
        FLUXGL_LOG_TRACE("Registry::destroyEntity marked for deletion: " + std::to_string(id));
        m_entitiesToDelete.insert(id);
    }

    bool Registry::isValidEntity(EntityID id) {
        return id > 0 && id <= FLUXGL_MAX_ENTITIES;
    }

    bool Registry::isAliveEntity(EntityID id) {
        return std::find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), id) == m_entitiesToDelete.end();
    }

    bool Registry::isEntityInUse(EntityID id) {
        return std::find(m_availableIDs.begin(), m_availableIDs.end(), id) == m_availableIDs.end();
    }

    void Registry::setParent(EntityID child, EntityID parent) {
        // Make sure that the child is valid
        if(!isValidEntity(child)) throw std::runtime_error("Registry::SetParent: Invalid child entity" + std::to_string(child));

        // If the parent is valid, set it as this child's parent
        if(isValidEntity(parent)) {
            m_hierarchy[child] = parent;
            return;
        }

        // If the parent is not valid, remove the child from the hierarchy system
        m_hierarchy.erase(child);
    }

    std::vector<Entity> Registry::getChildren(EntityID id) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::GetChildren: Invalid entity" + std::to_string(id));

        // Go through the hirerarchy and return all the children of the given entity
        std::vector<Entity> result;
        for(auto& [child, parent] : m_hierarchy) {
            if(parent == id) {
                result.push_back(Entity(child, this));
            }
        }

        return result;
    }

    Entity Registry::getParent(EntityID id) {
        if(!isValidEntity(id)) throw std::runtime_error("Registry::GetParent: Invalid entity" + std::to_string(id));

        // Get the parent of the given entity
        auto it = m_hierarchy.find(id);
        if(it == m_hierarchy.end()) return Entity(0, this);

        return Entity(it->second, this);
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