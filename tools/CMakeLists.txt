add_executable(AssetPacker
    asset_packer.cpp
)

function(generate_assets_pack target source_dir output_file)
    add_custom_command(
        OUTPUT ${output_file}
        COMMAND $<TARGET_FILE:AssetPacker>
            pack
            ${source_dir}
            ${output_file}
        DEPENDS AssetPacker ${source_dir}
        COMMENT "Packing Assets for ${target}"
        VERBATIM
    )
    
    add_custom_target(${target} ALL
        DEPENDS ${output_file}
    )
endfunction()