cmake_minimum_required(VERSION 3.28)

set(VERSION_MAJOR 4)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

project(OpenGL_Figures LANGUAGES C CXX VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

find_package(OpenGL REQUIRED)

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(PROJ_ARCHITECTURE x64)
else()
	set(PROJ_ARCHITECTURE x86)
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(PROJ_GRAPHIC_DIR D:/Sources/Graphic/OpenGL)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(PROJ_GRAPHIC_DIR /Users/user/Documents/Sources/Graphic/OpenGL)

	set(PROJ_LIBRARIES
		"-framework Cocoa"
		"-framework IOKit"
		"-framework QuartzCore")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(PROJ_GRAPHIC_DIR /home/serg/Documents/Sources/Graphic/OpenGL)
endif()

set(PROJ_TEMP_DIR ${CMAKE_SOURCE_DIR}/build/temp_sources)
set(PROJ_SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources/)
set(PROJ_SHADER_DIR ${PROJ_SOURCE_DIR}/shaders)
set(PROJ_CPP_SOURCE_DIR ${PROJ_SOURCE_DIR}/cpp_sources)

set(PROJ_SHADER_SOURCES 
	${PROJ_SHADER_DIR}/StaticObj_Color.vert
	${PROJ_SHADER_DIR}/Interpolation.frag
	${PROJ_SHADER_DIR}/DynamicInterpolation.frag
	${PROJ_SHADER_DIR}/DynamicObj.vert
	${PROJ_SHADER_DIR}/DynamicColor.frag
	${PROJ_SHADER_DIR}/StaticObj.vert
	${PROJ_SHADER_DIR}/DynamicColor_UniformColor.frag
)
set(PROJ_TEMP_SOURCES
	${PROJ_TEMP_DIR}/version.h
	${PROJ_TEMP_DIR}/version.cpp)
set(PROJ_SOURCES 
	${PROJ_CPP_SOURCE_DIR}/main.cpp
	${PROJ_CPP_SOURCE_DIR}/shader.h
	${PROJ_CPP_SOURCE_DIR}/shader.cpp
	${PROJ_CPP_SOURCE_DIR}/gl_utility.h
	${PROJ_CPP_SOURCE_DIR}/gl_utility.cpp
	${PROJ_CPP_SOURCE_DIR}/shape.h
	${PROJ_CPP_SOURCE_DIR}/shape.cpp
	${PROJ_CPP_SOURCE_DIR}/figures_data.h
	${PROJ_CPP_SOURCE_DIR}/figures_data.cpp
	${PROJ_SHADER_SOURCES})
set(PROJ_LIBRARIES ${PROJ_LIBRARIES}
	glfw3
	OpenGL::GL
)

set(GLAD2_INCLUDE ${PROJ_GRAPHIC_DIR}/GLAD2/include)
set(GLAD2_SOURCES ${PROJ_GRAPHIC_DIR}/GLAD2/src/glad.c)
set(GLFW_INCLUDE ${PROJ_GRAPHIC_DIR}/GLFW/include)
set(GLFW_LIB ${PROJ_GRAPHIC_DIR}/GLFW/lib/${PROJ_ARCHITECTURE}/$<CONFIG>)

set(PROJ_RUNTIME_BUILD_DIR ${CMAKE_SOURCE_DIR}/binary/${PROJ_ARCHITECTURE}/$<CONFIG>)

set(PROJ_SHADER_OUTPUT_DIR ${PROJ_RUNTIME_BUILD_DIR}/shaders)

set(PROJ_CONFIG_FILE_1_IN ${PROJ_CPP_SOURCE_DIR}/version.h_cmake_in)
set(PROJ_CONFIG_FILE_2_IN ${PROJ_CPP_SOURCE_DIR}/version.cpp_cmake_in)
set(PROJ_CONFIG_FILE_1_OUT ${PROJ_TEMP_DIR}/version.h)
set(PROJ_CONFIG_FILE_2_OUT ${PROJ_TEMP_DIR}/version.cpp)

get_property(PROJ_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(PROJ_IS_MULTI_CONFIG)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
endif()

option(PROJ_VERSION_PRINT "IF selected, project version will be displayed on configuration stage, otherwise nope" ON)
option(PROJ_VERSION_DISPL "IF selected, project version will be displayed on window title, otherwise nope" ON)

configure_file(${PROJ_CONFIG_FILE_1_IN} ${PROJ_CONFIG_FILE_1_OUT})
configure_file(${PROJ_CONFIG_FILE_2_IN} ${PROJ_CONFIG_FILE_2_OUT})

add_executable(${CMAKE_PROJECT_NAME}
	${PROJ_SOURCES}
	${GLAD2_SOURCES}
	${PROJ_TEMP_SOURCES}
	${PROJ_CONFIG_FILE_1_IN}
	${PROJ_CONFIG_FILE_2_IN}
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJ_SHADER_DIR} ${PROJ_SHADER_OUTPUT_DIR})

if(CMAKE_GENERATOR MATCHES "Visual Studio")
	target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup")
endif()
target_include_directories(${CMAKE_PROJECT_NAME} 
	PRIVATE ${GLAD2_INCLUDE} 
	PRIVATE ${GLFW_INCLUDE}
	PRIVATE ${PROJ_TEMP_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PROJ_LIBRARIES})
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLFW_LIB})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED YES)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJ_RUNTIME_BUILD_DIR})

if(PROJ_VERSION_PRINT)
	message("Project version: ${CMAKE_PROJECT_VERSION}")
endif()