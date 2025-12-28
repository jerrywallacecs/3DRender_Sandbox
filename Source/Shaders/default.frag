#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float emissiveStrength;

	float constant;
	float linear;
	float quadratic;

	float innerCutoff;
	float outerCutoff;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;
in vec2 TextureCoords;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void directionalLight()
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TextureCoords).rgb;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextureCoords).rgb;

	// specular
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	vec3 emission = light.emissiveStrength * texture(material.emission, TextureCoords).rgb;

	FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}

void pointLight()
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TextureCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextureCoords).rgb;

	// specular
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float spec  = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	// emission
	vec3 emission = light.emissiveStrength * texture(material.emission, TextureCoords).rgb;

	// attenuation
	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}

void spotLight()
{
	vec3 lightDirection = normalize(light.position - FragPosition);

	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TextureCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextureCoords).rgb;

	// specular
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float spec  = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	// emission
	vec3 emission = light.emissiveStrength * texture(material.emission, TextureCoords).rgb;

	// spotlight (soft edges)
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}

void main()
{
	spotLight();
}