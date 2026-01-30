cmake_minimum_required (VERSION 3.28)

project("RenderSandbox")

add_executable(RenderSandbox
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
	"Source/UnitCube.h" 
	"Source/UnitCube.cpp"
    )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RenderSandbox PROPERTY CXX_STANDARD 20)
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

target_link_libraries(RenderSandbox PRIVATE ThirdParty)

target_include_directories(RenderSandbox
	PRIVATE
	${CMAKE_SOURCE_DIR}/Source/Shaders
)

# handling assimp dll
add_custom_command(TARGET RenderSandbox POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/Libraries/DLL/assimp-vc143-mt.dll
	$<TARGET_FILE_DIR:RenderSandbox>
)
