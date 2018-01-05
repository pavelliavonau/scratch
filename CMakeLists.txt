cmake_minimum_required(VERSION 2.8)

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib/mingw72/${CMAKE_BUILD_TYPE})

project(scratch)

# TODO: add data files to project tree view
# file(GLOB DATA_FILES /data/shaders/*.glsl)

add_definitions(
    -DGLEW_STATIC
)

set(scratch_SRCS
)

add_subdirectory(src)
#add_subdirectory(data)
