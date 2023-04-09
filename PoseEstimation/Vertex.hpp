#pragma once
#include "Config.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 Texcoord;
	glm::vec3 Normal;
	int BoneIds[Config::MaxBoneInfluence];
	float Weights[Config::MaxBoneInfluence];
};
