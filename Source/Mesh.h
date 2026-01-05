#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path; // used to compare with other textures
};

class Mesh {
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(Shader& shader);
	void Draw(Shader& shader, Shader& outlineShader, glm::mat4& modelMatrix);

private:
	// render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

#endif