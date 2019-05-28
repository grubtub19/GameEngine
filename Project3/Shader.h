#pragma once

#include <glad/glad.h>
#include <string>
#include "Mtx.h"


class Shader {
public:
	enum class type { GEOMETRY, VERTEX, FRAGMENT };

	// the program ID
	unsigned int ID;

	//constructor
	Shader();

	Shader(const std::string &vertexPath, const std::string &fragmentPath);

	Shader(const std::string &vertexPath, const std::string &geomPath, std::string &fragmentPath);

	void readShaderSource(Shader::type const &type, std::string const &path, int &source, int &success, char *infoLog);

	void use();

	void setUniform1i(char const *uniform_name, int const &value);
	void setUniform1f(char const *uniform_name, float const &value);
	void setUniform3fv(char const *uniform_name, float const *array);
	void setUniform4fv(char const *uniform_name, float const *array);
	void setUniformMatrix4fv(char const *uniform_name, const mtx::Matrix4& matrix, bool transpose);
	void setUniformMatrix4fv(char const* uniform_name, const std::vector<mtx::Matrix4>& matrices, bool transpose);
};