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
	const auto nodeName = node.Name;
	auto nodeTransform = node.Transformation;

	if (const auto bone = CurrentAnimation->FindBone(nodeName))
	{
		bone->Update(CurrentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	const auto globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = CurrentAnimation->GetBoneIdMap();
	if (boneInfoMap.contains(nodeName))
	{
		const auto index = boneInfoMap[nodeName].Id;
		const auto offset = boneInfoMap[nodeName].Offset;
		FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node.ChildrenCount; i++)
		CalculateBoneTransform(node.Children[i], globalTransformation);
}
