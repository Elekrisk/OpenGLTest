#pragma once

#include <glad/glad.h>

#include <string>

// Denna klass �r n�stan helt kopierad fr�n en tutorial
class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	// Detta skrev jag sj�lv, s� att man ej beh�ver skriva shader.ID f�r att f� ID:t.
	operator unsigned int()
	{
		return ID;
	}
};