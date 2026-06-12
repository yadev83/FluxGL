/**
 * Current Asset Packer version : 1.0
 * Current Magic : FGLD
 * Format ============== 
 * [HEADER]
 *      string[4]   magic
 *      uint32      version
 *      uint32      file_count
 *      uint32      pkg_name_len
 * [PKG_NAME]
 * [FILE_INFOS]
 *      string path
 *      uint64 offset
 *      uint64 size
 * [FILE_DATA]
 *      char*   file1
 *      char*   file2
 *      ...
 */

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#include <cstdint>
#include <cstring>
#include <string>

struct PkgHeader {
    char        magic[4];
    uint32_t    version;
    uint32_t    file_count;
    uint32_t    pkg_name_len;
};

struct PkgFileEntry {
    std::string path;
    uint64_t    offset;
    uint64_t    size;
};

void create_asset_package(std::string path, std::string outputPath = "") {
    std::string packageName = outputPath.size() ? outputPath : path + ".fgld";
    std::cout << "Packaging folder " << path << " as " << packageName << std::endl;

    // Prepare FileEntries, by scanning the root folder
    std::vector<PkgFileEntry> entries;

    if(!std::filesystem::exists(path)) throw std::runtime_error("Asset directory does not exist: " + path);
    if(!std::filesystem::is_directory(path)) throw std::runtime_error("Not a directory: " + path);
    
    for(auto const& file : std::filesystem::recursive_directory_iterator(path)) {
        if(!file.is_regular_file()) continue;

        PkgFileEntry entry;
        entry.path = std::filesystem::relative(file.path(), path).generic_string();
        entry.size = std::filesystem::file_size(file.path());

        std::cout << "Created file entry " << entry.path << " (" << entry.size << " bytes)" << std::endl;
        entries.push_back(entry);
    }

    // Prepare the Data Zone, for this we need to know how long the fileEntry part will be
    uint64_t indexSize = 0;
    for(auto const& entry : entries) {
        indexSize += sizeof(uint32_t); // path length
        indexSize += entry.path.size();

        indexSize += sizeof(uint64_t); // offset
        indexSize += sizeof(uint64_t); // size
    }

    // Determine offsets for entries
    uint64_t currentOffset = sizeof(PkgHeader) + packageName.size() + indexSize;
    for(auto& entry : entries) {
        entry.offset = currentOffset;
        currentOffset += entry.size;
    }

    // We can now start to write the asset file
    std::filesystem::path outPath(packageName);
    if (outPath.has_parent_path()){
        std::filesystem::create_directories(outPath.parent_path());
    }
    std::ofstream out(packageName, std::ios::binary);

    PkgHeader header;
    memcpy(header.magic, "FGLD", 4);
    header.version = 1;
    header.file_count = entries.size();
    header.pkg_name_len = packageName.size();

    out.write(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    out.write(
        packageName.data(),
        packageName.size()
    );

    // Index entries write
    for(auto const& entry : entries) {
        uint32_t pathLen = static_cast<uint32_t>(entry.path.size());

        out.write(
            reinterpret_cast<char*>(&pathLen),
            sizeof(pathLen)
        );

        out.write(
            entry.path.data(),
            pathLen
        );

        out.write(
            reinterpret_cast<const char*>(&entry.offset),
            sizeof(entry.offset)
        );

        out.write(
            reinterpret_cast<const char*>(&entry.size),
            sizeof(entry.size)
        );
    }

    // Write data
    for(auto const& entry : entries) {
        std::ifstream file(
            path + "/" + entry.path,
            std::ios::binary
        );

        out << file.rdbuf();
    }
}

void open_asset_package(std::string path, std::string output) {
    std::ifstream in(path, std::ios::binary);
    if(!in) {
        throw std::runtime_error("Cannot open package");
    }

    PkgHeader header;
    in.read(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    if(std::strncmp(header.magic, "FGLD", 4) != 0) {
        throw std::runtime_error("Invalid package magic");
    }

    std::string packageName(header.pkg_name_len, '\0');

    in.read(
        packageName.data(),
        header.pkg_name_len
    );

    std::cout << "Package: " << packageName << std::endl;
    std::cout << "Files: " << header.file_count << std::endl;

    std::vector<PkgFileEntry> entries;
    for(uint32_t i = 0; i < header.file_count; ++i) {
        uint32_t pathLen;

        in.read(
            reinterpret_cast<char*>(&pathLen),
            sizeof(pathLen)
        );

        std::string path(pathLen, '\0');

        in.read(
            path.data(),
            pathLen
        );

        uint64_t offset;
        uint64_t size;

        in.read(
            reinterpret_cast<char*>(&offset),
            sizeof(offset)
        );

        in.read(
            reinterpret_cast<char*>(&size),
            sizeof(size)
        );

        entries.push_back({
            path,
            offset,
            size
        });
    }

    std::filesystem::path outputDir = output.size() ? output : std::filesystem::path(path).stem();
    std::filesystem::create_directories(outputDir);

    for(const auto& entry : entries) {
        std::cout
            << "Extracting "
            << entry.path
            << " (" << entry.size << " bytes)"
            << std::endl;

        in.seekg(entry.offset);
        std::vector<char> buffer(entry.size);

        in.read(
            buffer.data(),
            static_cast<std::streamsize>(entry.size)
        );

        auto outputPath = outputDir / entry.path;
        std::filesystem::create_directories(outputPath.parent_path());

        std::ofstream out(
            outputPath,
            std::ios::binary
        );

        out.write(
            buffer.data(),
            static_cast<std::streamsize>(entry.size)
        );
    }

    std::cout << "Extraction complete\n";
}

int main(int argc, char *argv[]) {
    try {
        if(argc < 3) {
            throw std::runtime_error("Wrong usage of asset packer. \nExpected usage is : AssetPacker.exe <mode=pack|unpack> <src_path> <output_path(optional)>");
        }

        std::string mode = argv[1];
        std::string source = argv[2];
        std::string output = argc > 3 ? argv[3] : "";

        if(mode == "pack") {
            create_asset_package(source, output);
        }  else if(mode == "unpack") {
            open_asset_package(source, output);
        } else {
            throw std::runtime_error("Unkown mode : " + mode + ". Accepted modes are : pack | unpack");
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }    

    return 0;
}