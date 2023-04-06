#pragma once
#include "Libs.hpp"
#include "Input.hpp"
#include "Shader.hpp"

class View
{
public:
	explicit View();
	void Update(const ImVec2& screenSize);
	void SendToShader(const Shader& shader) const;

	void SetToDefault();
	void OnMouseMove(double x, double y, const Button& button);

	[[nodiscard]] float* FocusPtr()	   { return reinterpret_cast<float*>(&Focus); }
	[[nodiscard]] float* DistancePtr() { return &Distance; }
	[[nodiscard]] int*	 FovPtr()	   { return &Fov; }
	[[nodiscard]] int*	 PitchPtr()	   { return &Pitch; }
	[[nodiscard]] int*	 YawPtr()	   { return &Yaw; }
	[[nodiscard]] int*	 RollPtr()	   { return &Roll; }

private:
	[[nodiscard]] glm::quat GetDirection() const;
	[[nodiscard]] glm::vec3 GetForward() const;

	void DefaultFocus()	   { Focus = { 0.0f, 0.0f, 0.0f }; }
	void DefaultDistance() { Distance = 5.0f; }
	void DefaultFov()	   { Fov = 45; }
	void DefaultPitch()	   { Pitch = 0; }
	void DefaultYaw()	   { Yaw = 0; }
	void DefaultRoll()	   { Roll = 0; }

	glm::mat4 ViewMatrix;
	glm::vec3 Forward;
	float Aspect, Near, Far;
	float RotationSpeed;
	glm::vec2 CurrentPos;

	glm::vec3 Focus;
	float Distance;
	int Fov;
	int Pitch, Yaw, Roll;
};
