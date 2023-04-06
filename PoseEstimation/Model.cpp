#include "Model.hpp"

void Model::Draw() const
{
	for(const auto& mesh : Meshes)
		mesh.Draw();
}

void Model::Update(const float deltaTime)
{
	UpdateModel();
	ModelAnimator.Update(deltaTime);
}

void Model::SendToShader(const Shader& shader)
{
	shader.SetI1(Config::MaxBoneInfluence, "MaxBoneInfluence");
	shader.SetMat4(ModelMatrix, "ModelMatrix");

	const auto transforms = ModelAnimator.GetFinalBoneMatrices();
	for (size_t i = 0; i < transforms.size(); ++i)
		shader.SetMat4(transforms[i], "FinalBonesMatrices[" + std::to_string(i) + "]");
}

void Model::UpdateModel()
{
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(glm::vec3(1.0f));
}