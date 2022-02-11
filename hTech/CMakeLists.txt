# CMakeList.txt : CMake project for Framework, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories(includes/hTech /inc)
include_directories(SDL/include)
include_directories(SDL/include)
include_directories(SDL/include)
include_directories(SDL/include)
LINK_DIRECTORIES("SDL")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source to this project's executable.
file(GLOB hTech_SRC CONFIGURE_DEPENDS "includes/hTech/*.h" "includes/hTech/*.cpp")

add_library (hTech SHARED ${hTech_SRC})
target_link_libraries(hTech SDL2 SDL2_gfx SDL2_image SDL2_ttf)

#[[ file(COPY includes DESTINATION ${CMAKE_BINARY_DIR}/lib) ]]
file(COPY includes DESTINATION ${CMAKE_BINARY_DIR}/lib FILES_MATCHING PATTERN *.h)
