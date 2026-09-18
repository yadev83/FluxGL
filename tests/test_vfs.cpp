#include <doctest/doctest.h>

#include <fluxgl/core/virtual_file_system.h>
#include <fluxgl/core/directory_storage_provider.h>
#include <fluxgl/core/error.h>

#include <filesystem>
#include <fstream>

namespace {
    std::filesystem::path makeTempFile(const std::string& name, const std::string& content) {
        auto dir = std::filesystem::temp_directory_path() / "fluxgl_tests";
        std::filesystem::create_directories(dir);
        auto path = dir / name;
        std::ofstream out(path, std::ios::binary);
        out.write(content.data(), static_cast<std::streamsize>(content.size()));
        out.close();
        return path;
    }
}

TEST_CASE("VFS: read and readText via DirectoryStorageProvider") {
    auto path = makeTempFile("vfs_hello.txt", "fluxgl\ntest");

    fluxgl::DirectoryStorageProvider provider;
    fluxgl::VirtualFileSystem vfs(&provider);

    CHECK(vfs.fileExists(path.string()));
    CHECK_FALSE(vfs.fileExists("definitely_missing_file.bin"));

    auto bytes = vfs.read(path.string());
    REQUIRE(bytes.size() == 11);
    CHECK(bytes[0] == 'f');

    CHECK(vfs.readText(path.string()) == "fluxgl\ntest");
}

TEST_CASE("VFS: reading a missing file throws IOError") {
    fluxgl::DirectoryStorageProvider provider;
    fluxgl::VirtualFileSystem vfs(&provider);

    CHECK_THROWS_AS(vfs.read("nope.bin"), fluxgl::Error);
    try {
        vfs.read("nope.bin");
        FAIL("expected throw");
    } catch(const fluxgl::Error& e) {
        CHECK(e.code == fluxgl::make_error_code(fluxgl::ErrorCode::IOError));
    }
}

TEST_CASE("VFS: no storage provider throws IOError") {
    fluxgl::VirtualFileSystem vfs;
    CHECK_THROWS_AS(vfs.read("anything.bin"), fluxgl::Error);
    CHECK_THROWS_AS(vfs.fileExists("anything.bin"), fluxgl::Error);
}

TEST_CASE("VFS: binary content round-trips") {
    auto path = makeTempFile("vfs_bin.bin",
                             std::string("\x00\x01\x02\xff\x10", 5));

    fluxgl::DirectoryStorageProvider provider;
    fluxgl::VirtualFileSystem vfs(&provider);

    auto bytes = vfs.read(path.string());
    REQUIRE(bytes.size() == 5);
    CHECK(bytes[0] == 0x00);
    CHECK(bytes[1] == 0x01);
    CHECK(bytes[3] == 0xFF);
}