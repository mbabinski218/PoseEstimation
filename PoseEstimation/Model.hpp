#pragma once

#include "glew/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ImageConverter.hpp"
#include "ObjLoader.hpp"
#include <filesystem>
#include "imgui.h"

class Model
{
public:
	// Variables
	std::unique_ptr<std::vector<Vertex>> Vertices;
	std::unique_ptr<ImageConverter> Image;

	glm::mat4 ModelMatrix;
	ImVec2 WindowSize;

	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	//Methods
	explicit Model(const std::string& modelObjPath, const ImVec2& windowSize);

private:
	void Init() const;
	void UpdateModelMatrix();
};