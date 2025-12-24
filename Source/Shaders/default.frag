#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D m_texture;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular lighting
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	// take the dot product of the view direction and reflection direction
	// then that the max of that to ensure it isn't negative. Then raise it
	// to the power of x. Note that x is the shininess value of the highlight
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// base color
	vec4 textureColor = texture(m_texture, TexCoord);

	vec3 result = textureColor.rgb * objectColor * (ambient + diffuse + specular);
	FragColor = vec4(result, textureColor.a);
}