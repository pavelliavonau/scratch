cmake_minimum_required(VERSION 3.0.0)

project(scratch)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(src)
add_subdirectory(data/shaders/gl)
add_subdirectory(data/shaders/gles)
add_subdirectory(data/models)
add_subdirectory(data/textures)
