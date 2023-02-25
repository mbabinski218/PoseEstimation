#pragma once

#include "glew/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ImageConverter.hpp"
#include "ObjLoader.hpp"
#include <filesystem>
#include "imgui.h"

class Model
{
	// Variables
	std::unique_ptr<std::vector<Vertex>> Vertices;
	std::unique_ptr<ImageConverter> Image;

	glm::mat4 ModelMatrix;
	GLuint VAO;
	GLuint VBO;

	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	//Methods
public:
	explicit Model(const std::string& modelObjPath);
	void* GetTexture() const;

private:
	void InitVAO();
	void Update();
	void UpdateModelMatrix();
};