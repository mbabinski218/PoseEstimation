#pragma once
#include "Mesh.hpp"
#include "Bone.hpp"
#include "Shader.hpp"
#include "Animator.hpp"

class Model
{
public:
	explicit Model(const int id) : ModelMatrix({1.0f}), Id(id) {}

	void Draw() const;
	void Update(float deltaTime);
	void SendToShader(const Shader& shader);
	void Animate(const std::shared_ptr<Animation>& animation) { ModelAnimator.Play(animation); }

	[[nodiscard]] int GetId() const { return Id; }
	[[nodiscard]] std::vector<Mesh>& GetMeshes() { return Meshes; }
	[[nodiscard]] int& GetBoneCounter() { return BoneCounter; }
	[[nodiscard]] std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() { return BoneInfoMap; }

private:
	void UpdateModel();

	Animator ModelAnimator{};
	std::vector<Mesh> Meshes{};

	glm::mat4 ModelMatrix{};
	std::unordered_map<std::string, BoneInfo> BoneInfoMap{};
	int BoneCounter = 0;
	int Id = -1;
};
