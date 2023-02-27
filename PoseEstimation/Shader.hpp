#pragma once

#include "glew/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/fwd.hpp"
#include <filesystem>
#include <exception>
#include <fstream>
#include <string>

class Shader
{
	// Variables
	GLuint Id;
	const int VersionMajor;
	const int VersionMinor;
	GLboolean Transpose;

	// Methods
public:
	Shader(const char* vertexFilePath, const char* fragmentFilePath);
	void Bind() const;
	void Unbind() const;
	void SetMat4Fv(glm::mat4& value, const GLchar* name) const;

private:
	std::string LoadShaderSource(const char* filePath) const;
	GLuint LoadShader(GLenum type, const char* filePath) const;
	void LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};
