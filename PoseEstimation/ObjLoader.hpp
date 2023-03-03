#pragma once

#include "glew/glew.h"
#include "Vertex.hpp"
#include <fstream>
#include <sstream>
#include <vector>

class ObjLoader
{
public:
	static std::vector<Vertex> Load(const std::string& path);
};
