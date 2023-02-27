#include "VertexIndexBuffer.hpp"

VertexIndexBuffer::VertexIndexBuffer(const std::vector<Vertex>& vertices) :
	VAO(0),
	VBO(0),
	VerticesCount(static_cast<int>(vertices.size()))
{
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));

    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));

    //glBindVertexArray(0);

	// Create VAO, VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO, VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Bind data
	glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
	glEnableVertexAttribArray(1);
	// Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Texcoord)));
	glEnableVertexAttribArray(2);
	// Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(3);

	// BIND VAO 0
	glBindVertexArray(0);
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
    glDrawElements(GL_TRIANGLES, VerticesCount, GL_UNSIGNED_INT, nullptr);
}