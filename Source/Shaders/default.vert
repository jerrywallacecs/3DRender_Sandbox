# version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// note that we read the multiplication from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;

	// note that the calculation below is getting the transpose 
	// of the upper-left 3x3 part of the model matrix

	// This is quite a costly operation for shaders and is
	// better handled by calculating the normal matrix on the
	// CPU and sending it to the shaders via a uniform 
	// (like how we are doing with the model matrix)

	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPosition = vec3(model * vec4(aPos, 1.0));
}