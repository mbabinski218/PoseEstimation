#include "Shader.hpp"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) :
	Id(0),
	VersionMajor(4),
	VersionMinor(4),
	Transpose(GL_FALSE)
{
	const auto currentPath = std::filesystem::current_path().string();

	const auto vertexShader = LoadShader(GL_VERTEX_SHADER, (currentPath + vertexFilePath).c_str());
	const auto fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (currentPath + fragmentFilePath).c_str());

	LinkProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::LoadShaderSource(const char* filePath) const
{
	std::string src;
	std::ifstream file;

	file.open(filePath);
	if (file)
	{
		std::string temp;
		while (std::getline(file, temp))
			src += temp + "\n";
	}
	else
	{
		throw std::exception("Shader file error");
	}
	file.close();

	const auto versionNr = std::to_string(VersionMajor) + std::to_string(VersionMinor) + "0";

	src.replace(src.find("#version"), 12, "#version " + versionNr);

	return src;
}

GLuint Shader::LoadShader(const GLenum& type, const char* filePath) const
{
	GLint success;

	const auto shader = glCreateShader(type);
	const auto strSrc = LoadShaderSource(filePath);
	const auto src = strSrc.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::exception(infoLog);
	}

	return shader;
}

void Shader::LinkProgram(const GLuint& vertexShader, const GLuint fragmentShader)
{
	GLint success;

	Id = glCreateProgram();

	glAttachShader(Id, vertexShader);
	glAttachShader(Id, fragmentShader);

	glLinkProgram(Id);
	glValidateProgram(Id);

	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(Id, 512, nullptr, infoLog);
		throw std::exception(infoLog);
	}

	glUseProgram(0);
}

void Shader::Bind() const
{
	glUseProgram(Id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetMat4(const glm::mat4& mat4, const std::string& name) const
{
	const auto myLoc = glGetUniformLocation(Id, name.c_str());
	glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetVec3(const glm::vec3& vec3, const std::string& name) const
{
	const auto myLoc = glGetUniformLocation(Id, name.c_str());
	glProgramUniform3fv(Id, myLoc, 1, glm::value_ptr(vec3));
}

void Shader::SetF1(const float value, const std::string& name) const
{
	const auto myLoc = glGetUniformLocation(Id, name.c_str());
	glUniform1f(myLoc, value);
}