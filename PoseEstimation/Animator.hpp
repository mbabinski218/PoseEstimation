#pragma once
#include "Animation.hpp"

class Animator
{
public:
	explicit Animator();
	void Play(const std::shared_ptr<Animation>& animation);
	void Update(float deltaTime);
	[[nodiscard]] std::vector<glm::mat4> GetFinalBoneMatrices() { return FinalBoneMatrices; }

private:
	void CalculateBoneTransform(const NodeData& node, const glm::mat4& parentTransform);

	std::shared_ptr<Animation> CurrentAnimation{};
	std::vector<glm::mat4> FinalBoneMatrices{};
	float CurrentTime = 0.0f;
};