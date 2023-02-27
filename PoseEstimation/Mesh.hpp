#pragma once

#include "glew/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ObjLoader.hpp"
#include <filesystem>
#include "FrameBuffer.hpp"
#include "Shader.hpp"
#include "imgui.h"
#include "VertexIndexBuffer.hpp"

class Mesh
{
	// Variables
	std::shared_ptr<Shader>& ModelShader;
	std::unique_ptr<FrameBuffer> FBuffer;
	std::unique_ptr<VertexIndexBuffer> VIBuffer;

	glm::mat4 ModelMatrix;
	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	//Methods
public:
	explicit Mesh(const std::string& modelObjPath, std::shared_ptr<Shader>& shader, const ImVec2& modelSize);
	void* GetTexture() const;
	void Update();

private:
	void UpdateModelMatrix();
	void UpdateUniforms();
};
