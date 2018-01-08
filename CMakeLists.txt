cmake_minimum_required(VERSION 2.8)

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib/mingw72/${CMAKE_BUILD_TYPE})

project(scratch)

if (UNIX)
	find_package(GLEW REQUIRED)
	if (GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif()

	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	target_link_libraries(scratch ${GLFW_LIBRARIES})
	target_link_libraries(scratch ${GLFW_STATIC_LIBRARIES})
endif (UNIX)

add_definitions(
	-DGLEW_STATIC
)

add_subdirectory(src)
add_subdirectory(include/glm)
