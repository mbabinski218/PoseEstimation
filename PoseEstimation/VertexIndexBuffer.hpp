#pragma once
#include "Vertex.hpp"

class VertexIndexBuffer
{
	// Variables
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;
	int VerticesCount;
	int IndicesCount;

	// Methods
public:
	explicit VertexIndexBuffer(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	~VertexIndexBuffer();
	void Bind() const;
	void Unbind() const;
	void Draw() const;
	void Clear() const;
};