#version 330 core
out vec4 FragColor;

in vec2 TextureCoords;

uniform sampler2D texture1;


void main()
{
	vec4 textureColor = texture(texture1, TextureCoords);
	FragColor = textureColor;
}