#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPosition;
in vec2 TextureCoords;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(viewPosition - FragPosition);

	// phase 1: Directional Lighting
	vec3 result = calculateDirectionalLight(directionalLight, norm, viewDirection);

	// phase 2: Point Lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pointLights[i], norm, FragPosition, viewDirection);
	}

	// phase 3: Spot Light
	result += calculateSpotLight(spotLight, norm, FragPosition, viewDirection);

	FragColor = vec4(result, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDirection), 0.0);

	// specular shading
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);

	// diffuse shading
	float diff = max(dot(normal, lightDirection), 0.0);

	// specular shading
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	// attenuation
	float distance = length(light.position - fragmentPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);

	// diffuse shading
	float diff = max(dot(normal, lightDirection), 0.0);

	// specular shading
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	// attenuation
	float distance = length(light.position - fragmentPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// spotlight intensity
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}