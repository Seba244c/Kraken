add_subdirectory(stb_image)
set_target_properties(stb_image PROPERTIES FOLDER "Libs")

set(MSDFGEN_USE_SKIA OFF)
set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_NO_ARTERY_FONT ON)
set(MSDF_ATLAS_DYNAMIC_RUNTIME ON)
set(MSDF_ATLAS_MSDFGEN_EXTERNAL ON)
add_subdirectory(msdf-atlas-gen)
set_target_properties(msdf-atlas-gen PROPERTIES FOLDER "Libs")

add_subdirectory("embed-resource")
