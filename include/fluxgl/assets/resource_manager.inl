#pragma once

#include <stdexcept>

namespace fluxgl {
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
            throw std::runtime_error("Resource with name '" + name + "' not found");
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
}