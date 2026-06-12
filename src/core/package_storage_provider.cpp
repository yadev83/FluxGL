#include <fluxgl/core/package_storage_provider.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>
#include <cstring>
#include <stdexcept>

namespace fs = std::filesystem;
namespace fluxgl {
    void PackageStorageProvider::loadPackage(const std::filesystem::path& filePath) {
        FLUXGL_LOG_DEBUG("Loading package : " + filePath.generic_string());
        // io checks/open file
        if(!fs::exists(filePath) || !fs::is_regular_file(filePath)) return;

        Package package;
        package.file.open(filePath, std::ios::binary);
        
        if(!package.file) throw Error{ErrorCode::IOError, "Failed to open package: " + filePath.generic_string()};

        // package file now opened => read header and populate the index
        struct Header {
            char magic[4];
            uint32_t version;
            uint32_t file_count;
            uint32_t pkg_name_len;
        } header;

        package.file.read(reinterpret_cast<char*>(&header), sizeof(header));

        if(std::strncmp(header.magic, "FGLD", 4) != 0) throw Error{ErrorCode::IOError, "Invalid package magic: " + filePath.generic_string()};

        std::string pkgName(header.pkg_name_len, '\0');
        package.file.read(pkgName.data(), header.pkg_name_len);

        for(uint32_t i = 0; i < header.file_count; ++i) {
            uint32_t pathLen = 0;
            package.file.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));

            std::string path(pathLen, '\0');
            package.file.read(path.data(), pathLen);
            
            PackageEntry entry{};
            package.file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
            package.file.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size));

            FLUXGL_LOG_DEBUG("file index: " + path + " (offset: " + std::to_string(entry.offset) + "B, size: " + std::to_string(entry.size) + "B)");
            package.entries.emplace(std::move(path), entry);
        }

        // key = assetsPath/pkgName (no extension so we can do read(assetsPath/pkgName/asset.xxx))
        std::string key = filePath.generic_string().substr(0, filePath.generic_string().find_last_of("."));
        package.path = filePath.generic_string();
        
        FLUXGL_LOG_DEBUG("Successfully read package index: " + package.path + " as " + key);
        m_packages.emplace(std::move(key), std::move(package));
    }

    PackageStorageProvider::PackageStorageProvider(std::string assetsPath, bool recursive) {
        fs::path rootPath(assetsPath);

        if(!fs::exists(rootPath)) throw Error{ErrorCode::IOError, "Assets path does not exist: " + assetsPath};

        if(recursive) {
            for(const auto& it : fs::recursive_directory_iterator(rootPath)) {
                if(!it.is_regular_file()) continue;
                loadPackage(it.path());
            }
        } else {
            loadPackage(rootPath);
        }
    }

    Buffer PackageStorageProvider::read(std::string path) {
        // Find package
        fs::path current(path);

        while(!current.empty()) {
            auto it = m_packages.find(current.generic_string());

            if(it != m_packages.end()) {
                Package& package = it->second;
                
                fs::path assetPath = fs::relative(
                    fs::path(path),
                    current
                );
                auto entryIt = package.entries.find(assetPath.generic_string());
                if(entryIt == package.entries.end()) throw Error{ErrorCode::IOError, "File not found in package: " + path};

                const PackageEntry& entry = entryIt->second;
                
                package.file.clear();
                package.file.seekg(static_cast<std::streamoff>(entry.offset), std::ios::beg);

                if(!package.file) throw Error{ErrorCode::IOError, "Encountered a problem while trying to read the file entry: " + assetPath.generic_string()};

                Buffer buffer(entry.size);
                package.file.read(
                    reinterpret_cast<char*>(buffer.data()),
                    static_cast<std::streamsize>(entry.size)
                );

                if(!package.file) throw Error{ErrorCode::IOError, "Could not read file: " + assetPath.generic_string()};

                return buffer;
            }

            current = current.parent_path();
        }
        
        throw Error{ErrorCode::IOError, "Package not found for path: " + path};
    }
}