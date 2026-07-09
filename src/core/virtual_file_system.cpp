#include <fluxgl/core/virtual_file_system.h>
#include <fluxgl/core/error.h>

namespace fluxgl {
    VirtualFileSystem::VirtualFileSystem(IStorageProvider *provider) : m_provider(provider) {}

    Buffer VirtualFileSystem::read(std::string path) {
        if(!m_provider) throw Error{ErrorCode::IOError, "VirtualFileSystem could not find storage provider"};
        return m_provider->read(path);
    }

    std::string VirtualFileSystem::readText(std::string path) {
        Buffer bytes = read(path);

        return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    }

    bool VirtualFileSystem::fileExists(std::string path) {
        if(!m_provider) throw Error{ErrorCode::IOError, "VirtualFileSystem could not find storage provider"};
        return m_provider->fileExists(path);
    }
}