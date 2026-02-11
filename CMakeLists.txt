cmake_minimum_required(VERSION 3.20)
project(FluxGL)

# C++23 is required for std::excepted<T, E>
set(CMAKE_CXX_STANDARD 23)

# Include dirs
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Find packages
find_package(glfw3 CONFIG REQUIRED)

# Build the library
add_library(FluxGL STATIC src/fluxgl.cpp)
target_link_libraries(FluxGL PRIVATE glfw)

# Build the example
add_executable(hello examples/hello.cpp)
target_link_libraries(hello PRIVATE FluxGL)
set_target_properties(hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)