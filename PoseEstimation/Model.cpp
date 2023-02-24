#include "Model.hpp"

Model::Model(const std::string& objFilePath, const ImVec2& windowSize) : Image(std::make_unique<ImageConverter>()),
	WindowSize(windowSize), MatrixId(0)
{
	const auto vertices = ObjLoader::Load(objFilePath);
	Vertices = std::make_unique<std::vector<Vertex>>(vertices);

	Init();
}

void* Model::GetTexture() const
{
	return Image->GetTexture();
}

glm::mat4 Model::ComputeMvp(const float& zoom, const float& rotateCamera, const glm::vec3& initPos, const float& y, const float& z) const
{
	const glm::mat4 projection = glm::perspective(glm::radians(zoom), WindowSize.x / WindowSize.y, 0.1f, 100.0f);

	const glm::vec3 rotate(cos(rotateCamera) * 10.0 - initPos.x, 0, sin(rotateCamera) * 10.0 - initPos.z);

	const glm::vec3 position = initPos + rotate;

	const glm::mat4 view =
		    glm::lookAt(position,          // Camera is at in World Space
			glm::vec3(0, 0, 0),  // Camera looking at (origin)
			glm::vec3(0, 1, 0)   // Head (0,-1,0 to look upside-down)
		);

	const glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, y, z));

	const glm::mat4 mvp = projection * view * model;
	return mvp;
}

void Model::Init() const
{
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices->size() * sizeof(glm::vec3),	&Vertices->at(0), GL_DYNAMIC_DRAW);

	glm::mat4 mvp = ComputeMvp(Zoom, RotateCamera, InitPos, Y, Z);

	glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

	glDrawArrays(GL_TRIANGLES, 0, Vertices->size());

	glDisableVertexAttribArray(0);

	Image->LoadMat(reinterpret_cast<void*>(MatrixId),  static_cast<int>(WindowSize.x), static_cast<int>(WindowSize.y));
}


void Model::Update(const ImVec2& windowSize) const
{
	
}
