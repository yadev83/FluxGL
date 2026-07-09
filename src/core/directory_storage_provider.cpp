#include <fluxgl/core/directory_storage_provider.h>
#include <fluxgl/core/error.h>

#include <filesystem>
#include <fstream>

namespace fluxgl {
    Buffer DirectoryStorageProvider::read(std::string path) {
        if(!std::filesystem::exists(path)) {
            throw Error{ErrorCode::IOError, "File not found: " + path};
        }

        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if(!file) {
            throw Error{ErrorCode::IOError, "Failed to open file: " + path};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        Buffer buffer(size);

        if(!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            throw Error{ErrorCode::IOError, "Failed to read file: " + path};
        }

        return buffer;
    }

    bool DirectoryStorageProvider::fileExists(std::string path) {
        return std::filesystem::exists(path);
    }
}