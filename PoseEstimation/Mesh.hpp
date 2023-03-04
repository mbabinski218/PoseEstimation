#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"
#include "FrameBuffer.hpp"
#include "Input.hpp"
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
	float RotationSpeed;
	glm::vec2 CurrentPos;

	glm::vec3 Focus;
	float Distance;
	int Fov;
	int Pitch, Yaw, Roll;
	
	//Methods
public:
	explicit Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath);
	void* GetTexture() const;
	void Render() const;
	void Update(const ImVec2& screenSize);
	void Reset();
	void OnMouseMove(const double& x, const double& y, const Button& button);

	void ResetFocus() { Focus = { 0.0f, 0.0f, 0.0f }; }
	void ResetDistance() { Distance = 5.0f; }
	void ResetFov() { Fov = 45.0f; }
	void ResetPitch() { Pitch = 0; }
	void ResetYaw() { Yaw = 0; }
	void ResetRoll() { Roll = 0; }

	float* FocusPtr() { return reinterpret_cast<float*>(&Focus); }
	float* DistancePtr() { return &Distance; }
	int* FovPtr() { return &Fov; }
	int* PitchPtr() { return &Pitch; }
	int* YawPtr() { return &Yaw; }
	int* RollPtr() { return &Roll; }

private:
	void UpdateViewMatrix();
	glm::quat GetDirection() const;
	glm::vec3 GetForward() const;
};
