project(Models)

file(GLOB Models_SRC "*.obj")

add_custom_target(${PROJECT_NAME} SOURCES ${Models_SRC})
