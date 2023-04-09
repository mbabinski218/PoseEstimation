#include "stdafx.h"
#include "Animator.hpp"
#include "Config.hpp"

Animator::Animator()
{
	FinalBoneMatrices.reserve(Config::MaxBones);

	for (auto i = 0; i < Config::MaxBones; i++)
		FinalBoneMatrices.emplace_back(1.0f);
}

void Animator::Play(const std::shared_ptr<Animation>& animation)
{
	CurrentAnimation = animation;
	CurrentTime = 0.0f;
}

void Animator::Update(const float deltaTime)
{
	if (CurrentAnimation)
	{
		CurrentTime += static_cast<float>(CurrentAnimation->GetTicksPerSecond()) * deltaTime;
		CurrentTime = fmod(CurrentTime, CurrentAnimation->GetDuration());
		CalculateBoneTransform(CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::CalculateBoneTransform(const NodeData& node, const glm::mat4& parentTransform)
{
	const auto nodeName = node.BoneName;

	const auto globalTransformation = parentTransform * node.Transformation;

	const auto boneInfoMap = CurrentAnimation->GetBoneIdMap();
	const auto iterator = boneInfoMap.find(nodeName);

	if (iterator != boneInfoMap.end())
	{
		const auto index = iterator->second.Id;
		const auto offset = iterator->second.Offset;
		FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node.ChildrenCount; i++)
		CalculateBoneTransform(node.Children[i], globalTransformation);
}
