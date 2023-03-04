#pragma once
#include "Libs.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 Texcoord;
	glm::vec3 Normal;

	bool operator==(const Vertex& other) const
	{
		return Position == other.Position
			&& Color == other.Color
			&& Normal == other.Normal
			&& Texcoord == other.Texcoord;
	}

	size_t operator()(const Vertex v) const
	{
		return std::hash<glm::vec3>()(v.Position)
			 ^ std::hash<glm::vec3>()(v.Color)
			 ^ std::hash<glm::vec2>()(v.Texcoord)
			 ^ std::hash<glm::vec3>()(v.Normal);
	}
};

namespace std
{
	template<>
	struct hash<Vertex>
	{
		size_t operator()(const Vertex& v) const
		{
			return std::hash<glm::vec3>()(v.Position)
				 ^ std::hash<glm::vec3>()(v.Color)
				 ^ std::hash<glm::vec2>()(v.Texcoord)
				 ^ std::hash<glm::vec3>()(v.Normal);
		}
	};
}