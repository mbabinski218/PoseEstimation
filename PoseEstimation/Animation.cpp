#include "Animation.hpp"
#include "Config.hpp"
#include "Utility.hpp"

void Fill(const BoneType boneType, NodeData& parentNode, const std::map<BoneType, glm::vec3> transformations)
{
	for(const auto& posePair : Config::PosePairs)
	{
		if(posePair[0] == boneType)
		{
			const auto iterator = transformations.find(posePair[1]);
			if(iterator != transformations.end())
			{
				parentNode.ChildrenCount++;

				auto node = NodeData{};
				node.Name = Converter::ToString(iterator->first);
				node.Transformation = glm::translate(glm::mat4(1.0f), iterator->second);
				Fill(iterator->first, node, transformations);

				parentNode.Children.push_back(node);
			}
		}
	}
}

std::shared_ptr<Animation> Animation::Create(const Skeleton& skeleton, const std::map<std::string, BoneInfo>& boneInfoMap, const float duration, const int ticksPerSecond)
{
	auto animation = Animation{};
	animation.BoneInfoMap = boneInfoMap;
	animation.Duration = duration;
	animation.TicksPerSecond = ticksPerSecond;

	const auto transformations = skeleton.GetBonesTransformations();
	const auto rootTransformation = transformations.begin();

	auto rootNode = NodeData{};
	rootNode.Name = Converter::ToString(rootTransformation->first);
	rootNode.Transformation = glm::translate(glm::mat4(1.0f), rootTransformation->second);
	Fill(rootTransformation->first, rootNode, transformations);

	animation.RootNode = rootNode;

	return std::make_shared<Animation>(animation);
}

Bone* Animation::FindBone(const std::string& name)
{
	const auto iterator = std::ranges::find_if(Bones, [&](const Bone& bone)
	{
		return bone.GetName() == name;
	});

	if (iterator == Bones.end())
		return nullptr;

	return &*iterator;
}