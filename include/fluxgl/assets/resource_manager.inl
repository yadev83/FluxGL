#pragma once

#include <stdexcept>

namespace fluxgl {
    template<typename T>
    ResourceStorage<T>::~ResourceStorage() {
        for (auto& slot : m_slots)
        {
            delete slot.resource;
            slot.resource = nullptr;
        }
    }

    template<typename T>
    ResourceHandle<T> ResourceStorage<T>::add(T* resource, const std::string& name) {
        // Find the first available slot
        for (size_t i = 0; i < m_slots.size(); ++i) {
            if (!m_slots[i].occupied) {
                // Update the selected slot
                m_slots[i].resource = resource;
                m_slots[i].occupied = true;

                // If a name is provided, map it to the index
                if (!name.empty()) {
                    m_nameToIndex[name] = i;
                }

                // Return a handle for the added resource
                return { i, m_slots[i].generation };
            }
        }

        // If no slot is available, add a new one
        m_slots.push_back({ resource, 1, true });

        // If a name is provided, map it to the new index
        if (!name.empty()) {
            m_nameToIndex[name] = m_slots.size() - 1;
        }

        // Return a handle for this resource
        return { m_slots.size() - 1, 1 };
    }

    template<typename T>
    T* ResourceStorage<T>::get(ResourceHandle<T> handle) {
        if(!handle.isValid()) return nullptr;
        if(handle.index >= m_slots.size()) return nullptr;

        auto& slot = m_slots[handle.index];
        if(!slot.occupied) return nullptr;
        if(slot.generation != handle.generation) return nullptr;

        return slot.resource;
    }

    template<typename T>
    ResourceHandle<T> ResourceStorage<T>::find(const std::string& name) {
        auto it = m_nameToIndex.find(name);

        if(it == m_nameToIndex.end()) {
            return { ResourceHandle<T>::INVALID_INDEX, 0 }; // Invalid handle returned
        }

        return { it->second, m_slots[it->second].generation };
    }

    template<typename T>
    std::vector<ResourceHandle<T>> ResourceStorage<T>::findAll() {
        std::vector<ResourceHandle<T>> handles;
        for(size_t i = 0; i < m_slots.size(); ++i) {
            if(m_slots[i].occupied) {
                handles.push_back({ i, m_slots[i].generation });
            }
        }

        return handles;
    }

    template<typename T>
    void ResourceStorage<T>::remove(ResourceHandle<T> handle) {
        if (handle.index < m_slots.size()) {
            ResourceSlot<T>& slot = m_slots[handle.index];

            if (slot.occupied && slot.generation == handle.generation) {
                delete slot.resource; // Assuming ownership of the resource
                slot.resource = nullptr;
                slot.occupied = false;
                slot.generation++; // Invalidate existing handles

                // Remove the name mapping if it exists
                for (auto it = m_nameToIndex.begin(); it != m_nameToIndex.end(); ++it) {
                    if (it->second == handle.index) {
                        m_nameToIndex.erase(it);
                        break;
                    }
                }
            }
        }
    }

    template<typename StorageType>
    ResourceStorage<StorageType>& ResourceManager::getStorage() {
        // Find the storage type index
        auto type = std::type_index(typeid(StorageType));

        // Grab the storage in the storages map
        auto it = m_storages.find(type);

        // If storage not found : create it and return it
        if(it == m_storages.end()) {
            auto* storage = new TypedStorage<StorageType>();
            m_storages[type] = storage;

            return storage->storage;
        }

        // Finally, return the storage properly if it was found
        return static_cast<TypedStorage<StorageType>*>(it->second)->storage;
    }

    template<typename ResourceType>
    ResourceHandle<ResourceType> ResourceManager::addResource(const std::string& name, ResourceType* itemPtr) {
        return getStorage<ResourceType>().add(itemPtr, name);
    }
    
    template<typename ResourceType>
    ResourceHandle<ResourceType> ResourceManager::addResource(const std::string& name, ResourceType item) {
        ResourceType* newResource = new ResourceType(std::move(item));
        return getStorage<ResourceType>().add(newResource, name);
    }
    
    template<typename ResourceType>
    ResourceHandle<ResourceType> ResourceManager::findResource(const Resource& resource) {
        return getStorage<ResourceType>().find(resource);
    }
    
    template<typename ResourceType>
    ResourceType* ResourceManager::getResource(ResourceHandle<ResourceType> handle) {
        return getStorage<ResourceType>().get(handle);
    }
    
    template<typename ResourceType>
    ResourceType* ResourceManager::getResource(const Resource& item) {
        return getResource<ResourceType>(findResource<ResourceType>(item));
        
    }
    
    template<typename ResourceType>
    std::vector<ResourceType*> ResourceManager::getResources(const std::vector<ResourceHandle<ResourceType>>& handles) {
        std::vector<ResourceType*> resources;
        resources.reserve(handles.size());

        for(const auto& handle : handles) {
            ResourceType* resource = getStorage<ResourceType>().get(handle);
            if(resource) {
                resources.push_back(resource);
            } else {
                resources.push_back(nullptr); // Or handle this case as needed
            }
        }

        return resources;
    }
    
    template<typename ResourceType>
    std::vector<ResourceType*> ResourceManager::getResources(const std::vector<Resource>& resources) {
        std::vector<ResourceHandle<ResourceType>> handles;
        for(Resource resource : resources) {
            handles.push_back(findResource<ResourceType>(resource));
        }

        return getResources<ResourceType>(handles);
    }
}