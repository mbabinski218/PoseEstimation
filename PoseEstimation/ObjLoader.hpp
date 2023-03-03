#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"

class ObjLoader
{
public:
	static void Load(const std::string& path, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};
