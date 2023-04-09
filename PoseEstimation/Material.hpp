#pragma once
#include "Shader.hpp"

struct Material
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	explicit Material(const glm::vec3& ambient = glm::vec3(0.1f), 
	                  const glm::vec3& diffuse = glm::vec3(1.0f),
	                  const glm::vec3& specular = glm::vec3(1.0f)) :
	Ambient(ambient),
	Diffuse(diffuse),
	Specular(specular) { }

	void SendToShader(const Shader& shader) const
	{
		shader.SetVec3(Ambient, "material.ambient");
		shader.SetVec3(Diffuse, "material.diffuse");
		shader.SetVec3(Specular, "material.specular");
	}
};
