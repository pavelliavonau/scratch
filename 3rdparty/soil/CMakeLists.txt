project(SOIL C)

################################################################################
# Source groups
################################################################################
set(Header_Files
	src/image_DXT.h
	src/image_helper.h
	src/SOIL.h
	src/stb_image_aug.h
	src/stbi_DDS_aug.h
	src/stbi_DDS_aug_c.h
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
	src/image_DXT.c
	src/image_helper.c
	src/SOIL.c
	src/stb_image_aug.c
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES ${Source_Files} ${Header_Files})

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

################################################################################
# Output directory
################################################################################
string(CONCAT OUT_DIR
    ${CMAKE_SOURCE_DIR}/lib/$<CONFIG>/
)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})

################################################################################
# Compile definitions
################################################################################

target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB;UNICODE;_UNICODE")

################################################################################
# Compile and link options
################################################################################
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		  $<$<CONFIG:Debug>:/Od;/RTC1;/MDd;/W3;/Zi;/EHsc;/Y->
		$<$<CONFIG:Release>:/GL;/MD;/W3;/Zi;/EHsc;/Y->
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE "/LTCG")
endif()
