function(add_gui_executable name path)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    add_executable(${name} ${path})

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
        else()
            set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE FALSE) # keep pour debug
        endif()
    endif()

    target_link_libraries(${name} PRIVATE FluxGL)
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/${name})
    
    list(GET path 0 first_source)
    get_filename_component(source_dir ${first_source} PATH)
    if(exists ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets $<TARGET_FILE_DIR:${name}>/assets
        )
    endif()
endfunction()

# Build the examples
add_gui_executable(hello hello/hello.cpp)
add_gui_executable(textures textures/textures.cpp)
add_gui_executable(transform transform/transform.cpp)
add_gui_executable(3d 3d/3d.cpp)
add_gui_executable(light light/light.cpp)