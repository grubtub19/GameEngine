#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "Shader.h"
#include "Logger.h"

Shader::Shader() : ID(0) {}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {

	//create the shader in OpenGL
	ID = glCreateProgram();

	//declare one copy of variables needed to check the success of reading the shaders. Reduces redundancy.
	int success;
	char infoLog[1024];

	//declare shader source here, since we want to delete it only after the shader is linked
	int fragment = -1, vertex = -1;

	//retrieve the relative path

	std::filesystem::path relativePath = std::filesystem::current_path();

	//Logger::log(std::string("Relative Fragment Path: ") + relativePath.string() + "/" + fragmentPath);
	//Logger::log(std::string("Relative Vertex Path: ") + relativePath.string() + "/" + vertexPath);

	//reads and attaches shader source
	Shader::readShaderSource(Shader::type::FRAGMENT, relativePath.string() + "/" + fragmentPath, fragment, success, infoLog);
	Shader::readShaderSource(Shader::type::VERTEX, relativePath.string() + "/" + vertexPath, vertex, success, infoLog);

	//links shader with its source files
	glLinkProgram(ID);

	//If the linking was unsuccessful, prints the error log
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Shader::ID, 1024, NULL, infoLog);
		Logger::log(std::string("ERROR::SHADER::SHADER::COMPILATION_FAILED\n") + infoLog + "\n");
	}

	//deletes the shader sources since we don't need them anymore.
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

Shader::Shader(const std::string &vertexPath, const std::string &geometryPath, std::string &fragmentPath) {

	//create the shader in OpenGL
	ID = glCreateProgram();

	//declare one copy of variables needed to check the success of reading the shaders. Reduces redundancy.
	int success;
	char infoLog[1024];

	//declare shader source here, since we want to delete it only after the shader is linked
	int fragment = -1, vertex = -1, geometry = -1;

	//reads and attaches shader source
	Shader::readShaderSource(Shader::type::FRAGMENT, fragmentPath, fragment, success, infoLog);
	Shader::readShaderSource(Shader::type::VERTEX, vertexPath, vertex, success, infoLog);
	Shader::readShaderSource(Shader::type::GEOMETRY, geometryPath, geometry, success, infoLog);

	//links shader with its source files
	glLinkProgram(ID);

	//checks for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Shader::ID, 1024, NULL, infoLog);
		Logger::log(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
		exit(EXIT_FAILURE);
	}

	//deletes the shader sources since we don't need them anymore.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

/**
 * Reads a shader source of a certain type from a specified path
 * Source, success, and infoLog will contain info after execution
 */
void Shader::readShaderSource(Shader::type const &type, std::string const &path, int &source, int &success, char *infoLog) {

	//uses an input file stream to open the source file
	std::ifstream sourceFile;
	sourceFile.open(path);
	if (sourceFile.fail()) {
		Logger::log(std::string("Failed to open file: ") + path);
		exit(EXIT_FAILURE);
	}

	//takes the read ifstream's buffer and puts it into a stringstream
	std::stringstream sourceStream;
	sourceStream << sourceFile.rdbuf();
	sourceFile.close();

	std::string code = sourceStream.str();
	const char* sourceCode = code.c_str();
	std::string type_name;
	//creates the shader depending on its type
	switch (type) {
	case Shader::type::VERTEX:
		source = glCreateShader(GL_VERTEX_SHADER);
		type_name = "vertex";
		break;
	case Shader::type::FRAGMENT:
		source = glCreateShader(GL_FRAGMENT_SHADER);
		type_name = "fragment";
		break;
	case Shader::type::GEOMETRY:
		source = glCreateShader(GL_GEOMETRY_SHADER);
	}

	//adds the source code to the shader
	glShaderSource(source, 1, &sourceCode, NULL);

	//compiles shader
	glCompileShader(source);

	//checks for success of the compilation
	glGetShaderiv(source, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(source, 1024, NULL, infoLog);
		Logger::log(std::string(std::string("ERROR::SHADER::SOURCE::COMPILATION_FAILED for ") + type_name + "\n") + infoLog + "\n");
		std::cin.ignore();
		exit(EXIT_FAILURE);
	}

	//attaches the shader to the shader program
	glAttachShader(Shader::ID, source);
}

void Shader::use() {
	glUseProgram(Shader::ID);
}

void Shader::setUniform1i(char const *uniform_name, int const &value) {
	glUniform1i(glGetUniformLocation(ID, uniform_name), value);
}

void Shader::setUniform1f(char const *uniform_name, float const &value) {
	glUniform1f(glGetUniformLocation(ID, uniform_name), value);
}

void Shader::setUniform3fv(char const *uniform_name, float const *array) {
	glUniform3fv(glGetUniformLocation(ID, uniform_name), 1, array);
}

void Shader::setUniform4fv(char const *uniform_name, float const *array) {
	glUniform4fv(glGetUniformLocation(ID, uniform_name), 1, array);
}

void Shader::setUniformMatrix4fv(char const *uniform_name, const mtx::Matrix4& matrix, bool transpose) {
	glUniformMatrix4fv(glGetUniformLocation(ID, uniform_name), 1, transpose, matrix.array);
}

/**
 * Sends Matrix4 array to the GPU
 * @param matrices std::vector<mtx::Matrix4> the vector of matrices. Must contain at least 1 matrix.
 */
void Shader::setUniformMatrix4fv(char const* uniform_name, const std::vector<mtx::Matrix4>& matrices, bool transpose) {
	glUniformMatrix4fv(glGetUniformLocation(ID, uniform_name), matrices.size(), transpose, matrices.at(0).array);
}