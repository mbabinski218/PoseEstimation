#pragma once
#include "Libs.hpp"
#include "Model.hpp"
#include "Input.hpp"
#include "Material.hpp"
#include "LightPoint.hpp"
#include "FrameBuffer.hpp"

class World
{
public:
	explicit World();

	void Draw() const;
	void Update(const ImVec2& screenSize, float deltaTime);
	void OnMouseMove(double x, double y, const Button& button);
	void SetToDefault();
	void AddModel(int modelId) { Models.emplace_back(modelId); }
	void RemoveModel(int modelId);
	[[nodiscard]] void* GetTexture() const { return Frame.GetTexture(); }
	[[nodiscard]] Model* GetModel(int id);

	[[nodiscard]] float* LightPositionPtr() { return reinterpret_cast<float*>(&LightPosition); }
	[[nodiscard]] float* FocusPtr()			{ return reinterpret_cast<float*>(&Focus); }
	[[nodiscard]] float* DistancePtr()		{ return &Distance; }
	[[nodiscard]] int*   FovPtr()			{ return &Fov; }
	[[nodiscard]] int*   PitchPtr()			{ return &Pitch; }
	[[nodiscard]] int*   YawPtr()			{ return &Yaw; }
	[[nodiscard]] int*   RollPtr()			{ return &Roll; }

private:
	void UpdateViewMatrix();
	void SendToShader();

	[[nodiscard]] glm::quat GetDirection() const;
	[[nodiscard]] glm::vec3 GetForward() const;

	void DefaultFocus()			{ Focus = { 0.0f, 0.0f, 0.0f }; }
	void DefaultDistance()		{ Distance = 5.0f; }
	void DefaultFov()			{ Fov = 45; }
	void DefaultPitch()			{ Pitch = 0; }
	void DefaultYaw()			{ Yaw = 0; }
	void DefaultRoll()			{ Roll = 0; }
	void DefaultLightPosition() { LightPosition = { 3.0f, 2.0f, 7.0f }; }

	std::vector<Model> Models{};
	FrameBuffer Frame;
	Shader WorldShader;

	LightPoint Light{};
	Material Material{};

	glm::mat4 ViewMatrix;
	glm::vec3 Forward;
	float Aspect, Near, Far;
	float RotationSpeed;
	glm::vec2 CurrentPos;

	glm::vec3 LightPosition;
	glm::vec3 Focus;
	float Distance;
	int Fov;
	int Pitch, Yaw, Roll;
};
