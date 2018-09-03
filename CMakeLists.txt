cmake_minimum_required(VERSION 3.0.0)

project(scratch)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(include;3rdparty)

if (WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/lib/mingw72/${CMAKE_BUILD_TYPE})
endif (WIN32)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if (UNIX)
	find_package(PkgConfig REQUIRED)
	find_package(glfw3 3.2 REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	link_libraries(${GLFW_LIBRARIES})
	link_libraries(${GLFW_STATIC_LIBRARIES})
endif (UNIX)

add_subdirectory(3rdparty/soil)
add_subdirectory(3rdparty/glm)

add_subdirectory(src)
add_subdirectory(data/shaders/gl)
add_subdirectory(data/shaders/gles)
add_subdirectory(data/models)
add_subdirectory(data/textures)
