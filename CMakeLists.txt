cmake_minimum_required(VERSION 3.20)
project(FluxGL)

# Function definitions
function(add_gui_executable name)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    add_executable(${name} ${ARG_UNPARSED_ARGUMENTS})

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
        else()
            set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE FALSE) # keep pour debug
        endif()
    endif()

    target_link_libraries(${name} PRIVATE FluxGL)
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
    
    list(GET ARG_UNPARSED_ARGUMENTS 0 first_source)
    get_filename_component(source_dir ${first_source} PATH)
    add_custom_command(TARGET ${name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets $<TARGET_FILE_DIR:${name}>/assets
    )
endfunction()

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

# Include dirs
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Build the library
add_library(FluxGL STATIC 
    src/core/window.cpp
    src/core/error.cpp
    src/core/log.cpp
    src/utils/file.cpp
    src/graphics/mesh.cpp
    src/graphics/shader.cpp
    src/graphics/renderer.cpp
)
target_link_libraries(FluxGL 
    PUBLIC  glfw
            glad::glad
            glm::glm
)

# Build the examples
add_gui_executable(hello examples/hello/hello.cpp)