#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"
#include "FrameBuffer.hpp"
#include "Shader.hpp"
#include "VertexIndexBuffer.hpp"

class Mesh
{
	// Variables
	std::unique_ptr<Shader> ModelShader;
	std::unique_ptr<FrameBuffer> FBuffer;
	std::unique_ptr<VertexIndexBuffer> VIBuffer;

	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;

	glm::mat4 ModelMatrix;

	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	
	//Methods
public:
	explicit Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath, const ImVec2& modelSize);
	void* GetTexture() const;
	void Update();
	void Render() const;

	void Test(GLFWwindow* window);

private:
	void UpdateModelMatrix();
};
