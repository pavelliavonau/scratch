set(scratch_SRCS
	main.cpp
	UserDebugInfo.cpp
	Camera.cpp
	render/Shader.cpp
	Scene/Scene.cpp
	Scene/SceneObject.cpp
	Scene/Mesh.cpp
	Scene/TexturedMesh.cpp
	Scene/TexturedBox.cpp
)

file(GLOB shaders_SRC
	"../data/shaders/*.glsl"
)

include_directories(render)
include_directories(Scene)
include_directories(.)

add_executable(${PROJECT_NAME} ${DATA_FILES} ${scratch_SRCS} ${shaders_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

target_link_libraries(${PROJECT_NAME}
	glfw3
	glew32
	SOIL
	opengl32
	gdi32
	glu32
)
