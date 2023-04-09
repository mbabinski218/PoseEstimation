#include "stdafx.h"
#include "View.hpp"

View::View() :
	ViewMatrix(glm::mat4(0.0f)),
	Forward(glm::vec3(0.0f, 0.0f, -0.5f)),
	Aspect(1.3f),
	Near(0.1f), Far(100.0f),
	RotationSpeed(100.0f),
	CurrentPos(glm::vec2(0.0f, 0.0f))
{
	SetToDefault();
}

void View::SendToShader(const Shader& shader) const
{
	shader.SetMat4(ViewMatrix, "ViewMatrix");
	shader.SetMat4(glm::perspective(glm::radians<float>(static_cast<float>(Fov)), Aspect, Near, Far), "ProjectionMatrix");
	shader.SetVec3(glm::vec3(0.0f, 0.0f, 3.0f), "cameraPos");
}

void View::Update(const ImVec2& screenSize)
{
	Aspect = screenSize.x / screenSize.y;

	const auto position = Focus + glm::vec3(0.0f, 0.873f, 0.0f) - GetForward() * Distance;
	const glm::quat orientation = GetDirection();

	ViewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	ViewMatrix = glm::inverse(ViewMatrix);
}

glm::quat View::GetDirection() const
{
	const auto pitchInRadians = glm::radians<float>(static_cast<float>(Pitch));
	const auto yawInRadians = glm::radians<float>(static_cast<float>(Yaw));
	const auto rollInRadians = glm::radians<float>(static_cast<float>(Roll)) + glm::radians(180.0f);

	return glm::quat{ glm::vec3{-pitchInRadians, -yawInRadians, rollInRadians} };
}

glm::vec3 View::GetForward() const
{
	return rotate(GetDirection(), Forward);
}

void View::OnMouseMove(const double x, const double y, const Button& button)
{
	const auto pos = glm::vec2(x, y);

	if (button == Button::RIGHT)
	{
		//const auto delta = (pos - CurrentPos) * 0.004f;
		//const auto temp = Rotation.y + delta.x * RotationSpeed;

		//if (temp >= 180.0f)
		//	Rotation.y = 180.0f;
		//else if (temp <= -180.0f)
		//	Rotation.y = -180.0f;
		//else
		//	Rotation.y = temp;
	}

	CurrentPos = pos;
}

void View::SetToDefault()
{
	DefaultFov();
	DefaultDistance();
	DefaultPitch();
	DefaultYaw();
	DefaultRoll();
	DefaultFocus();
}