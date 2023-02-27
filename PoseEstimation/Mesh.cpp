#include "Mesh.hpp"

Mesh::Mesh(const std::string& modelObjPath, std::shared_ptr<Shader>& shader, const ImVec2& modelSize) :
	ModelShader(shader),
	FBuffer(std::make_unique<FrameBuffer>(modelSize.x, modelSize.y)),
	ModelMatrix(),
	Position(glm::vec3(0.f)),
	Origin(glm::vec3(0.f)),
	Rotation(glm::vec3(0.f)), Scale(glm::vec3(1.f))
{
	const auto currentPath = std::filesystem::current_path().string();
	const auto vertices = ObjLoader::Load(currentPath + modelObjPath);
	VIBuffer = std::make_unique<VertexIndexBuffer>(vertices);
}

void* Mesh::GetTexture() const
{
	return FBuffer->GetTexture();
}

void Mesh::UpdateUniforms()
{
	ModelShader->SetMat4Fv(ModelMatrix, "ModelMatrix");
}

void Mesh::UpdateModelMatrix()
{
	ModelMatrix = glm::mat4(1.f);
	ModelMatrix = glm::translate(ModelMatrix, Origin);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::translate(ModelMatrix, Position - Origin);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Mesh::Update()
{
	UpdateModelMatrix();

	ModelShader->Bind();

	UpdateUniforms();

	FBuffer->Bind();
	VIBuffer->Bind();

	VIBuffer->Draw();

	// Cleanup
	VIBuffer->Unbind();
	FBuffer->Unbind();
	ModelShader->Unbind();
}