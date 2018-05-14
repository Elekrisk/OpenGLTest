#pragma once

#include <glad/glad.h>

#include <string>

// Denna klass är nästan helt kopierad från en tutorial
class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	// Detta skrev jag själv, så att man ej behöver skriva shader.ID för att få ID:t.
	operator unsigned int()
	{
		return ID;
	}
};