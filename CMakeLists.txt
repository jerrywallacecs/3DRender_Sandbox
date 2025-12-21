cmake_minimum_required (VERSION 3.28)

project("OpenGL_Framework")

add_executable(OpenGL_Framework "Source/main.cpp" "Source/main.h" "Source/External/glad.c" "Source/External/stb_image.h" "Source/Shaders/Shader.h" "Source/Shaders/Shader.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL_Framework PROPERTY CXX_STANDARD 20)
endif()

add_library(ThirdParty INTERFACE)

target_include_directories(ThirdParty
	INTERFACE
	${CMAKE_SOURCE_DIR}/Libraries/Include
)

target_link_libraries(ThirdParty
	INTERFACE
	${CMAKE_SOURCE_DIR}/Libraries/Lib/glfw3.lib
)

target_link_libraries(OpenGL_Framework PRIVATE ThirdParty)

target_include_directories(OpenGL_Framework
	PRIVATE
	${CMAKE_SOURCE_DIR}/Source/Shaders
)
