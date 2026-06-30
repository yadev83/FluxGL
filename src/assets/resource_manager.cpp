#include <fluxgl/assets/resource_manager.h>

namespace fluxgl {
    ResourceManager::~ResourceManager() {
        for (auto& [type, storage] : m_storages) {
            delete storage;
        }

        m_storages.clear();
    }
}