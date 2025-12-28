#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float emissiveStrength;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;
in vec2 TextureCoords;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
	// ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, TextureCoords).rgb;

	// diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextureCoords).rgb;

	// specular lighting
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	// take the dot product of the view direction and reflection direction
	// then that the max of that to ensure it isn't negative. Then raise it
	// to the power of x. Note that x is the shininess value of the highlight
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	vec3 emission = light.emissiveStrength * texture(material.emission, TextureCoords).rgb;

	FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}