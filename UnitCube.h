#ifndef UNITCUBE_H
#define UNITCUBE_H

#include <glm/glm.hpp>
#include <string>
#include <Shader.h>

class UnitCube
{
public:
	unsigned int textureID;

	UnitCube(unsigned int textureID);

	void Draw(Shader& shader);
private:
	unsigned int VAO, VBO, EBO;
};

#endif