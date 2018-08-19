project(GLESShaders)

file(GLOB shaders_SRC "*.glsl")

add_custom_target(${PROJECT_NAME} SOURCES ${shaders_SRC})
