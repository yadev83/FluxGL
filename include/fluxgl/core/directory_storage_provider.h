#pragma once

#include "virtual_file_system.h"

namespace fluxgl {
    class DirectoryStorageProvider : public IStorageProvider {
        public:
            Buffer read(std::string path) override;
    };
}