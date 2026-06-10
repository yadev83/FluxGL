#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace fluxgl {
    using Buffer = std::vector<uint8_t>;

    class IStorageProvider {
        public:
            virtual Buffer read(std::string path) = 0;
    };

    class VirtualFileSystem {
        private:
            IStorageProvider* m_provider = nullptr;

        public:
            VirtualFileSystem(IStorageProvider *provider = nullptr);
            Buffer read(std::string path);
    };
}