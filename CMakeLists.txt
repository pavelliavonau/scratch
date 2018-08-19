cmake_minimum_required(VERSION 2.8)

include_directories(include)

if (WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/lib/mingw72/${CMAKE_BUILD_TYPE})
endif (WIN32)

project(scratch)

if (UNIX)
	find_package(GLEW REQUIRED)
	if (GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif()

	find_package(glfw3 3.2 REQUIRED)
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
	link_libraries(${OPENGL_gl_LIBRARY})
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	link_libraries(${GLFW_LIBRARIES})
	link_libraries(${GLFW_STATIC_LIBRARIES})
endif (UNIX)

add_definitions(
	-DGLEW_STATIC
)

add_subdirectory(src)
add_subdirectory(data/shaders/gl)
add_subdirectory(data/shaders/gles)
add_subdirectory(include/glm)
