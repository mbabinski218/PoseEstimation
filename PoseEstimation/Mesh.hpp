#pragma once
#include "Libs.hpp"
#include "Vertex.hpp"
#include "VertexIndexBuffer.hpp"

class Mesh
{
public:
	explicit Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) :
		Buffer(std::make_shared<VertexIndexBuffer>(vertices, indices)) {}

	void Draw() const
	{
		Buffer->Bind();
		Buffer->Draw();
		Buffer->Unbind();
	}

private:
	std::shared_ptr<VertexIndexBuffer> Buffer;
};
