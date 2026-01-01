cmake_minimum_required (VERSION 3.28)

project("OpenGL_Lessons")

add_executable(OpenGL_Lessons
	"Source/main.cpp"
	"Source/main.h"
	"Source/Camera.hpp"
	"Source/External/glad.c" 
	"Source/External/stb_image.h" 
	"Source/Shaders/Shader.h" 
	"Source/Shaders/Shader.cpp"
	"Source/Mesh.h" 
	"Source/Mesh.cpp" 
	"Source/Model.h" 
	"Source/Model.cpp"
	"Source/Textures/Texture.cpp" 
	"Source/Textures/Texture.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL_Lessons PROPERTY CXX_STANDARD 20)
endif()

add_library(ThirdParty INTERFACE)

target_include_directories(ThirdParty
	INTERFACE
	${CMAKE_SOURCE_DIR}/Libraries/Include
)

target_link_libraries(ThirdParty
	INTERFACE
	${CMAKE_SOURCE_DIR}/Libraries/Lib/glfw3.lib
	${CMAKE_SOURCE_DIR}/Libraries/Lib/assimp-vc143-mt.lib
)

target_link_libraries(OpenGL_Lessons PRIVATE ThirdParty)

target_include_directories(OpenGL_Lessons
	PRIVATE
	${CMAKE_SOURCE_DIR}/Source/Shaders
)

# handling assimp dll
add_custom_command(TARGET OpenGL_Lessons POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/Libraries/DLL/assimp-vc143-mt.dll
	$<TARGET_FILE_DIR:OpenGL_Lessons>
)
