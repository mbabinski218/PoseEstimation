#include "Model.hpp"



Model::Model(const std::string& modelObjPath, const ImVec2& windowSize) :
	Image(std::make_unique<ImageConverter>()),
	WindowSize(windowSize),
	Position(glm::vec3(0.f)), Origin(glm::vec3(0.f)), Rotation(glm::vec3(0.f)),
	Scale(glm::vec3(1.f))
{
	const auto currentPath = std::filesystem::current_path().string();
	const auto vertices = ObjLoader::Load(currentPath + modelObjPath);
	Vertices = std::make_unique<std::vector<Vertex>>(vertices);

	Init();
}

void Model::Init() const
{
	
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