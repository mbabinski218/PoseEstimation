#include "VertexIndexBuffer.hpp"

VertexIndexBuffer::VertexIndexBuffer(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) :
	VAO(0),
	EBO(0),
	VBO(0),
	VerticesCount(static_cast<int>(vertices.size())),
	IndicesCount(static_cast<int>(indices.size()))
{
	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Create VBO	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Create EBO
	if (IndicesCount > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
	glEnableVertexAttribArray(0);
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	// Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Texcoord)));
	glEnableVertexAttribArray(2);
	// Color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
	glEnableVertexAttribArray(3);

	// BIND VAO 0
	glBindVertexArray(0);
}

VertexIndexBuffer::~VertexIndexBuffer()
{
	Clear();
}

void VertexIndexBuffer::Bind() const
{
    glBindVertexArray(VAO);
}

void VertexIndexBuffer::Unbind() const
{
    glBindVertexArray(0);
}

void VertexIndexBuffer::Draw() const
{
	if (IndicesCount == 0)
		glDrawArrays(GL_TRIANGLES, 0, VerticesCount);
	else
		glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, nullptr);
}

void VertexIndexBuffer::Clear() const
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
