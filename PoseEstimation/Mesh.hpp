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
	glm::vec3 Forward;
	float Aspect, Near, Far;

public:
	glm::vec3 Focus;
	glm::vec3 Position;
	float Fov;
	int Pitch, Yaw, Roll;
	float Distance;
	
	//Methods
	explicit Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath);
	void* GetTexture() const;
	void Render() const;
	void Update();
	void ResetFov();
	void ResetPitch();
	void ResetYaw();
	void ResetRoll();
	void ResetDistance();
	void ResetFocus();
	void ResetAll();

private:
	void UpdateViewMatrix();
	glm::quat GetDirection() const;
	glm::vec3 GetForward() const;
};
