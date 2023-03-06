#pragma once
#include "Libs.hpp"

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

	void SetVec3(const glm::vec3& vec3, const std::string& name) const;
	void SetMat4(const glm::mat4& mat4, const std::string& name) const;
	void SetF1(const float& value, const std::string& name) const;

private:
	std::string LoadShaderSource(const char* filePath) const;
	GLuint LoadShader(const GLenum& type, const char* filePath) const;
	void LinkProgram(const GLuint& vertexShader, GLuint fragmentShader);
};
