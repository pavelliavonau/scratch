cmake_minimum_required(VERSION 3.13.0)

project(scratch)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
if(WIN32)
    find_package(SOIL REQUIRED)
endif()

add_subdirectory(src)
add_subdirectory(data/shaders/gl)
add_subdirectory(data/shaders/gles)
add_subdirectory(data/models)
add_subdirectory(data/textures)
