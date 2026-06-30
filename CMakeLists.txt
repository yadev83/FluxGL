cmake_minimum_required(VERSION 3.20)
project(FluxGL)

# Options
option(FLUXGL_BUILD_EXAMPLES "Build examples" OFF)
option(FLUXGL_PACK_ASSETS "Pack Assets" OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(APP_ICON_RESOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc"
)

# Macro in the code
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(FLUXGL_DEBUG=1)
else()
    add_compile_definitions(FLUXGL_DEBUG=0)
    set(FLUXGL_PACK_ASSETS ON CACHE BOOL "" FORCE)
    set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
endif()

if(FLUXGL_PACK_ASSETS)
    add_compile_definitions(FLUXGL_USE_ASSET_PACKS=1)
else()
    add_compile_definitions(FLUXGL_USE_ASSET_PACKS=0)
endif()

# C++23 is required for std::excepted<T, E>
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# Build the library
add_library(FluxGL STATIC)
add_library(FluxGL::FluxGL ALIAS FluxGL)

target_sources(FluxGL PRIVATE
    src/core/app.cpp
    src/core/window.cpp
    src/core/error.cpp
    src/core/log.cpp
    src/core/scene.cpp
    src/core/scene_manager.cpp
    src/core/virtual_file_system.cpp
    src/core/directory_storage_provider.cpp
    src/core/package_storage_provider.cpp
    src/ecs/registry.cpp
    src/ecs/entity.cpp
    src/ecs/behavior.cpp
    src/ecs/systems/render_system.cpp
    src/ecs/systems/lifetime_system.cpp
    src/ecs/systems/physics_system.cpp
    src/input/input_manager.cpp
    src/graphics/mesh.cpp
    src/graphics/shader.cpp
    src/graphics/renderer.cpp
    src/graphics/texture.cpp
    src/graphics/debug_renderer.cpp
    src/audio/audio_impl.cpp
    src/audio/sound.cpp
    src/assets/resource_manager.cpp
    src/assets/prefab_loader.cpp
)

target_include_directories(FluxGL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(FluxGL 
    PUBLIC  
        glfw
        glad::glad
        glm::glm
        JsonCpp::JsonCpp
)

# Build tools
add_subdirectory(tools)

# Build examples
if(FLUXGL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()