cmake_minimum_required(VERSION 3.20)

find_package(doctest CONFIG REQUIRED)

add_executable(fluxgl_tests
    test_main.cpp
    test_ecs.cpp
    test_math.cpp
    test_physics.cpp
    test_follow.cpp
    test_scene.cpp
    test_vfs.cpp
    test_ui.cpp
    test_input.cpp
)

target_compile_definitions(fluxgl_tests PRIVATE FLUXGL_BUILD_CORE=1)

target_link_libraries(fluxgl_tests PRIVATE
    fluxgl_core
    doctest::doctest
)

include(CTest)

# doctestConfig.cmake n'inclut que les cibles → charger le module qui définit
# doctest_discover_tests (indispensable pour lister chaque cas dans CTest)
include("${doctest_DIR}/doctest.cmake")

doctest_discover_tests(fluxgl_tests)