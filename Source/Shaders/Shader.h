#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // required OpenGL headers

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

	// utility functions - queries uniform location and sets value
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif