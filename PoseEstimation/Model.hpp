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
	ImVec2 WindowSize;
	GLint MatrixId;

	glm::vec3 InitPos = glm::vec3(4, 3, 3);
	float RotateCamera = 0.0f;
	float Zoom = 45.0f;
	float Y = 0.0f;
	float Z = 0.0f;

	//Methods
	explicit Model(const std::string& modelObjPath, const ImVec2& windowSize);
	void* GetTexture() const;
	void Update(const ImVec2& windowSize) const;

private:
	void Init() const;
	glm::mat4 ComputeMvp(const float& zoom, const float& rotateCamera, const glm::vec3& initPos, const float& y, const float& z) const;
};