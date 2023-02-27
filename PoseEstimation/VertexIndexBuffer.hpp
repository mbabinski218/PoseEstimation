#pragma once

#include "glew/glew.h"
#include <vector>
#include "Vertex.hpp"

class VertexIndexBuffer
{
	// Variables
	GLuint VAO;
	GLuint VBO;
	int VerticesCount;

	// Methods
public:
	explicit VertexIndexBuffer(const std::vector<Vertex>& vertices);
	void Bind() const;
	void Unbind() const;
	void Draw() const;
};