#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"
#include "FrameBuffer.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "VertexIndexBuffer.hpp"
#include "LightPoint.hpp"
#include "Material.hpp"

class Mesh
{
	// Variables
	std::unique_ptr<VertexIndexBuffer> Buffer;

	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;

	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::vec3 Forward;
	float Aspect, Near, Far;
	float RotationSpeed;
	glm::vec2 CurrentPos;

	LightPoint Light;
	Material Material;

	// Model
	glm::vec3 Rotation;
	glm::vec3 Scale;

	// Camera
	glm::vec3 LightPosition;
	glm::vec3 Focus;
	float Distance;
	int Fov;
	int Pitch, Yaw, Roll;
	
	//Methods
public:
	explicit Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	explicit Mesh(const Mesh& copy);

	void Update(const ImVec2& screenSize, std::shared_ptr<Shader> shader);
	void Draw() const;
	void SetToDefault();
	void OnMouseMove(double x, double y, const Button& button);

	void DefaultFocus() { Focus = { 0.0f, 0.0f, 0.0f }; }
	void DefaultDistance() { Distance = 5.0f; }
	void DefaultFov() { Fov = 45; }
	void DefaultPitch() { Pitch = 0; }
	void DefaultYaw() { Yaw = 0; }
	void DefaultRoll() { Roll = 0; }
	void DefaultLightPosition() { LightPosition = {3.0f, 2.0f, 7.0f}; }
	void DefaultRotation() { Rotation = { 0.0f, 0.0f, 0.0f }; }
	void DefaultScale() { Scale = { 1.0f, 1.0f, 1.0f }; }

	float* LightPositionPtr() { return reinterpret_cast<float*>(&LightPosition); }
	float* FocusPtr() { return reinterpret_cast<float*>(&Focus); }
	float* DistancePtr() { return &Distance; }
	int* FovPtr() { return &Fov; }
	int* PitchPtr() { return &Pitch; }
	int* YawPtr() { return &Yaw; }
	int* RollPtr() { return &Roll; }
	float* RotationPtr() { return reinterpret_cast<float*>(&Rotation); }
	float* ScalePtr() { return reinterpret_cast<float*>(&Scale); }

private:
	void UpdateModel();
	void UpdateViewMatrix();
	[[nodiscard]] glm::quat GetDirection() const;
	[[nodiscard]] glm::vec3 GetForward() const;
};
