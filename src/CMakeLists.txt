set(scratch_SRCS
	main.cpp
	render/shader.cpp
		scene/scene.cpp
		scene/sceneObject.cpp
		scene/BlinkingTriangle.cpp
		scene/TexturedBox.cpp
)

file(GLOB shaders_SRC
	"../data/shaders/*.glsl"
)

include_directories(render)
include_directories(scene)

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
