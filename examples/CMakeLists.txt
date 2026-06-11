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

    set(asset_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets)
    set(output_file ${CMAKE_BINARY_DIR}/examples/${name}/assets.fgld)
    set(ASSET_PACKER_EXE $<TARGET_FILE:AssetPacker>)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets)
        if(FLUXGL_PACK_ASSETS)
            add_custom_command(TARGET ${name} POST_BUILD
                COMMAND ${ASSET_PACKER_EXE}
                    pack
                    ${asset_dir}
                    ${output_file}
                COMMENT "Packing assets for ${name}"
            )
        else()
            add_custom_command(TARGET ${name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/assets $<TARGET_FILE_DIR:${name}>/assets
            )
        endif()
    endif()
endfunction()

# Build the examples
add_gui_executable(example01-hello hello/hello.cpp)
add_gui_executable(example02-textures textures/textures.cpp)
add_gui_executable(example03-transform transform/transform.cpp)
add_gui_executable(example04-3d 3d/3d.cpp)
add_gui_executable(example05-light light/light.cpp)
add_gui_executable(example06-audio audio/audio.cpp)
add_gui_executable(example07-sprites sprites/sprites.cpp)