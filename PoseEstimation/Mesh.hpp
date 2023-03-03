#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"
#include "FrameBuffer.hpp"
#include "Shader.hpp"
#include "VertexIndexBuffer.hpp"
#include "ObjLoader.hpp"

class Mesh
{
	// Variables
	std::unique_ptr<Shader> ModelShader;
	std::unique_ptr<FrameBuffer> FBuffer;
	std::unique_ptr<VertexIndexBuffer> VIBuffer;

	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;

	glm::mat4 ViewMatrix;

	
	//Methods
public:
	explicit Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath, const ImVec2& modelSize);
	void* GetTexture() const;
	void Render() const;
	void Update();

private:
	void UpdateViewMatrix();
};
