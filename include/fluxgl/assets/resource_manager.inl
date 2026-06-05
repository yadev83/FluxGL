#pragma once

namespace fluxgl {
    template<typename T>
    ResourceHandle<T> ResourceStorage<T>::add(T* resource) {
        // Find the first available slot
        for (size_t i = 0; i < m_slots.size(); ++i) {
            if (!m_slots[i].occupied) {
                m_slots[i].resource = resource;
                m_slots[i].generation++;
                m_slots[i].occupied = true;
                return { i, m_slots[i].generation };
            }
        }

        // If no slot is available, add a new one
        m_slots.push_back({ resource, 1, true });
        return { m_slots.size() - 1, 1 };
    }

    template<typename T>
    T* ResourceStorage<T>::get(ResourceHandle<T> handle) {
        if (handle.index < m_slots.size()) {
            ResourceSlot<T>& slot = m_slots[handle.index];
            if (slot.occupied && slot.generation == handle.generation) {
                return slot.resource;
            }
        }

        return nullptr; // Invalid handle
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
            }
        }
    }
}