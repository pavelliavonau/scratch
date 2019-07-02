project(scratch)

set(scratch_SRCS
	main.cpp
	UserDebugInfo.cpp
	Camera.cpp
	ImportObj.cpp
	render/Shader.cpp
	render/ShaderManager.cpp
	Scene/Scene.cpp
	Scene/SceneObject.cpp
	Scene/Mesh.cpp
	Scene/TexturedMesh.cpp
	Scene/TexturedBox.cpp
	Scene/Light.cpp
)

add_executable(${PROJECT_NAME} ${DATA_FILES} ${scratch_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC render;Scene;.)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${SOIL_INCLUDE_DIRS}>
)

add_definitions(
	-DGLEW_STATIC
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

add_dependencies(${PROJECT_NAME} SOIL)

target_link_libraries(${PROJECT_NAME}
	SOIL
	glm
	GLEW::GLEW
	OpenGL::GL
	glfw
)
