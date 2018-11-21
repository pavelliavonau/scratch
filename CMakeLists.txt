cmake_minimum_required(VERSION 3.0.0)

project(scratch)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(3rdparty)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Use GLFW
if (UNIX)
	find_package(PkgConfig REQUIRED)
	find_package(glfw3 3.2 REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
elseif (WIN32)
	set(GLFW_INCLUDE_DIRS ${CMAKE_PREFIX_PATH}/include)
	set(GLFW_LIBRARIES glfw3)
	link_directories(${CMAKE_PREFIX_PATH}/lib/${CMAKE_BUILD_TYPE}/)
endif ()

add_subdirectory(3rdparty/soil)
add_subdirectory(3rdparty/glm)

add_subdirectory(src)
add_subdirectory(data/shaders/gl)
add_subdirectory(data/shaders/gles)
add_subdirectory(data/models)
add_subdirectory(data/textures)
