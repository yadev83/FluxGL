#include <fluxgl/utils/file.h>
#include <fluxgl/core/error.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fluxgl {
    std::string readFileToString(const std::string& filePath) {
        if(!std::filesystem::exists(filePath)) {
            throw Error{ErrorCode::IOError, "File does not exist: " + filePath};
        }

        std::ifstream file(filePath);
        if(!file.is_open()) {
            throw Error{ErrorCode::IOError, "Failed to open file: " + filePath};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}