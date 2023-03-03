#include "Shader.hpp"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) :
	Id(0),
	VersionMajor(4),
	VersionMinor(4),
	Transpose(GL_FALSE)
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	const auto currentPath = std::filesystem::current_path().string();

	vertexShader = LoadShader(GL_VERTEX_SHADER, (currentPath + vertexFilePath).c_str());
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (currentPath + fragmentFilePath).c_str());

	LinkProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::LoadShaderSource(const char* filePath) const
{
	std::string temp = "";
	std::string src = "";
	std::ifstream inFile;

	inFile.open(filePath);
	if (inFile.is_open())
	{
		while (std::getline(inFile, temp))
			src += temp + "\n";
	}
	else
	{
		throw std::exception("Shader file error");
	}
	inFile.close();

	const std::string versionNr =	std::to_string(VersionMajor) +	std::to_string(VersionMinor) +	"0";

	src.replace(src.find("#version"), 12, ("#version " + versionNr));

	return src;
}

GLuint Shader::LoadShader(GLenum type, const char* filePath) const
{
	char infoLog[512];
	GLint success;

	const GLuint shader = glCreateShader(type);
	const std::string strSrc = LoadShaderSource(filePath);
	const GLchar* src = strSrc.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::exception(infoLog);
	}

	return shader;
}

void Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	char infoLog[512];
	GLint success;

	Id = glCreateProgram();

	glAttachShader(Id, vertexShader);
	glAttachShader(Id, fragmentShader);

	glLinkProgram(Id);
	glValidateProgram(Id);

	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success)
	{
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
	const GLint myLoc = glGetUniformLocation(Id, name.c_str());
	glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetVec3(const glm::vec3& vec3, const std::string& name) const
{
	const GLint myLoc = glGetUniformLocation(Id, name.c_str());
	glProgramUniform3fv(Id, myLoc, 1, glm::value_ptr(vec3));
}

void Shader::SetF1(const float& v, const std::string& name) const
{
	const GLint myLoc = glGetUniformLocation(Id, name.c_str());
	glUniform1f(myLoc, v);
}