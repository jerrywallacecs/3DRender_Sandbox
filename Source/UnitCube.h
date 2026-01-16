#ifndef UNITCUBE_H
#define UNITCUBE_H

#include <glm/glm.hpp>
#include <Shader.h>

class UnitCube
{
public:
	unsigned int textureID;

	UnitCube(unsigned int textureID);

	void Draw(Shader& shader);
	void Draw(Shader& shader, Shader& outlineShader, glm::mat4 modelMatrix);
private:
	unsigned int VAO, VBO, EBO;
};

#endif