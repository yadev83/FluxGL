#pragma once

#include "virtual_file_system.h"

#include <unordered_map>
#include <filesystem>
#include <fstream>

namespace fluxgl {
    struct PackageEntry {
        uint64_t offset;
        uint64_t size;
    };

    struct Package {
        std::ifstream file;
        std::string path;

        std::unordered_map<std::string, PackageEntry> entries;
    };

    class PackageStorageProvider : public IStorageProvider {
        private:
            std::unordered_map<std::string, Package> m_packages;
            void loadPackage(const std::filesystem::path& filePath);

        public:
            /**
             * @brief Construct a new Package Storage Provider object
             * And loads into packages either the given assetsPackagePath or the different packages in the folder if recursive
             * 
             * @param assetsPath The path to the assets folder or package
             * @param recursive If set to true, assetsPath is assumed to be a folder containing fluxgl packages
             */
            PackageStorageProvider(std::string assetsPath, bool recursive = false);

            /**
             * @brief Reads an asset into memory
             * 
             * @param path The path is always the full path, will be parsed around '/' and the first word will be considered 
             * as a package name to look for
             * @return Buffer 
             */
            Buffer read(std::string path) override;

            bool fileExists(std::string path) override;
    };
}