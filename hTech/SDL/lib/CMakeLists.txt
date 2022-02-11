#message("-- Linking SDL")
#
#add_library(SDL2 SHARED IMPORTED SDL2.dll)
#set_target_properties(SDL2 PROPERTIES LINKER_LANGUAGE C)
#
#add_library(SDL2_GFX SHARED IMPORTED  SDL2_gfx.dll)
#set_target_properties(SDL2_GFX PROPERTIES LINKER_LANGUAGE C)
#
#add_library(SDL2_IMAGE  SHARED IMPORTED SDL2_image.dll)
#set_target_properties(SDL2_IMAGE PROPERTIES LINKER_LANGUAGE C)
#
#add_library(SDL2_TTF SHARED IMPORTED  SDL2_ttf.dll)
#set_target_properties(SDL2_TTF PROPERTIES LINKER_LANGUAGE C)