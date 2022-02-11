message("-- Linking SDL")

add_library(SDL2 SDL2.dll)
set_target_properties(SDL2 PROPERTIES LINKER_LANGUAGE C)

add_library(SDL2_GFX SDL2_gfx.dll)
set_target_properties(SDL2_GFX PROPERTIES LINKER_LANGUAGE C)

add_library(SDL2_IMAGE SDL2_image.dll)
set_target_properties(SDL2_IMAGE PROPERTIES LINKER_LANGUAGE C)

add_library(SDL2_TTF SDL2_ttf.dll)
set_target_properties(SDL2_TTF PROPERTIES LINKER_LANGUAGE C)