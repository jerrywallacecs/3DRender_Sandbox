#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D m_texture;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	vec4 textureColor = texture(m_texture, TexCoord);
	vec3 result = textureColor.rgb * objectColor * lightColor;
	FragColor = vec4(result, textureColor.a);
}