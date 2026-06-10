#include <fluxgl/core/virtual_file_system.h>

namespace fluxgl {
    VirtualFileSystem::VirtualFileSystem(IStorageProvider *provider) : m_provider(provider) {}

    Buffer VirtualFileSystem::read(std::string path) {
        if(!m_provider) return Buffer(' ');
        return m_provider->read(path);
    }
}