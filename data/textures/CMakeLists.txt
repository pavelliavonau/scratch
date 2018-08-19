project(Textures)

file(GLOB Textures_png_SRC "*.png")
file(GLOB Textures_jpg_SRC "*.jpg")

add_custom_target(${PROJECT_NAME} SOURCES ${Textures_png_SRC} ${Textures_jpg_SRC})
