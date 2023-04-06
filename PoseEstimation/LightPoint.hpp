#pragma once
#include "Libs.hpp"
#include "Shader.hpp"

struct LightPoint
{
	float Intensity;
	glm::vec3 Color;
	glm::vec3 Position;
	float Constant;
	float Linear;
	float Quadratic;

	explicit LightPoint(const glm::vec3& position = glm::vec3(0.0f),
	                    const float intensity = 1.0f, 
	                    const glm::vec3& color = glm::vec3(1.0f),
	                    const float constant = 1.0f, 
	                    const float linear = 0.045f, 
	                    const float quadratic = 0.0075f) :
	Intensity(intensity),
	Color(color),
	Position(position),
	Constant(constant),
	Linear(linear),
	Quadratic(quadratic) { }

	void SendToShader(const Shader& shader) const
	{
		shader.SetVec3(Position, "pointLight.position");
		shader.SetF1(Intensity, "pointLight.intensity");
		shader.SetVec3(Color, "pointLight.color");
		shader.SetF1(Constant, "pointLight.constant");
		shader.SetF1(Linear, "pointLight.linear");
		shader.SetF1(Quadratic, "pointLight.quadratic");
	}

	void SetPosition(const glm::vec3 position) { Position = position; }
};
