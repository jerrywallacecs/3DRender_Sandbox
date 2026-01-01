#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID; // unique ID

	Shader(const char* vertexPath, const char* fragmentPath);
	void Activate();

	// utility functions - queries uniform locations and sets values
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& matrix) const;
	void setVec3(const std::string& name, const glm::vec3& vector) const;

	// commonly used matrices
	void setModelMatrix(const glm::mat4& matrix) const;
	void setViewMatrix(const glm::mat4& matrix) const;
	void setProjectionMatrix(const glm::mat4& matrix) const;
};

#endif