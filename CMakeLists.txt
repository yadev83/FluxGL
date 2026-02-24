cmake_minimum_required(VERSION 3.20)
project(FluxGL)

# Options
option(FLUXGL_BUILD_EXAMPLES "Build examples" OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Macro in the code
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(FLUXGL_DEBUG=1)
else()
    add_compile_definitions(FLUXGL_DEBUG=0)
endif()

# C++23 is required for std::excepted<T, E>
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

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
    src/ecs/registry.cpp
    src/ecs/entity.cpp
    src/ecs/behavior.cpp
    src/ecs/systems/render_system.cpp
    src/input/input_manager.cpp
    src/utils/file.cpp
    src/utils/compute.cpp
    src/graphics/mesh.cpp
    src/graphics/shader.cpp
    src/graphics/renderer.cpp
    src/graphics/texture.cpp
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
)

# Build examples
if(FLUXGL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()