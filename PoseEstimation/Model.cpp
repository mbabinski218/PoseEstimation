#include "Model.hpp"

Model::Model(const std::string& modelObjPath) :
	Image(std::make_unique<ImageConverter>()),
	ModelMatrix(),
	VAO(0),	VBO(0),
	Position(glm::vec3(0.f)),
	Origin(glm::vec3(0.f)),
	Rotation(glm::vec3(0.f)), Scale(glm::vec3(1.f))
{
	const auto currentPath = std::filesystem::current_path().string();
	const auto vertices = ObjLoader::Load(currentPath + modelObjPath);
	Vertices = std::make_unique<std::vector<Vertex>>(vertices);

	InitVAO();
}

void* Model::GetTexture() const
{
	return Image->GetTexture();
}

void Model::InitVAO()
{
	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices->size() * sizeof(Vertex), Vertices->data(), GL_STATIC_DRAW);

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

void Model::UpdateModelMatrix()
{
	ModelMatrix = glm::mat4(1.f);
	ModelMatrix = glm::translate(ModelMatrix, Origin);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::translate(ModelMatrix, Position - Origin);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Model::Update()
{
	UpdateModelMatrix();
}
