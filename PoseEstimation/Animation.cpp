#include "stdafx.h"
#include "Animation.hpp"
#include "Config.hpp"

Animation::Animation(const Skeleton& skeleton, const std::unordered_map<std::string, BoneInfo>& boneInfoMap, const float duration, const int ticksPerSecond) :
	Duration(duration), TicksPerSecond(ticksPerSecond), BoneInfoMap(std::move(boneInfoMap))
{
	const auto transformations = skeleton.GetBonesTransformations();
	const auto rootNodeTransformation = transformations.begin();

	auto rootNode = NodeData{};
	rootNode.Type = rootNodeTransformation->first;
	rootNode.BoneName = Config::BoneTypeNames.at(rootNodeTransformation->first);
	rootNode.Transformation = glm::translate(glm::mat4(1.0f), rootNodeTransformation->second);
	Fill(rootNodeTransformation->first, rootNode, transformations);
	RootNode = rootNode;

	ReadMissingBones(RootNode);
	auto vec = GetBones(-1, BoneInfoMap.find(rootNode.BoneName)->second.Id);
	RootNode.Children.insert(RootNode.Children.end(), vec.begin(), vec.end());
	RootNode.ChildrenCount += static_cast<int>(vec.size());
}

void Animation::Fill(const BoneType boneType, NodeData& parentNode, const std::map<BoneType, glm::vec3>& transformations)
{
	for(const auto& posePair : Config::PosePairs)
	{
		if(posePair[0] == boneType)
		{
			const auto pairIterator = transformations.find(posePair[1]);
			if(pairIterator != transformations.end())
			{
				parentNode.ChildrenCount++;

				auto node = NodeData{};
				node.Type = pairIterator->first;
				node.BoneName = Config::BoneTypeNames.at(pairIterator->first);
				node.Transformation = glm::translate(glm::mat4(1.0f), pairIterator->second);
				Fill(pairIterator->first, node, transformations);

				parentNode.Children.push_back(node);
			}
		}
	}
}

std::vector<NodeData> Animation::GetBones(const int minId, const int maxId)
{
	auto vec = std::vector<NodeData>{};

	auto subrange = BoneInfoMap | std::ranges::views::filter([&](const std::pair<std::string, BoneInfo>& boneInfo)
	{
		return boneInfo.second.Id > minId && boneInfo.second.Id < maxId;
	});

	for (const auto& elementKey : subrange | std::views::keys)
	{
		auto node = NodeData{};
		node.Type = UNKNOWN;
		node.BoneName = elementKey;
		node.Transformation = glm::mat4(1.0f);

		vec.push_back(node);
	}

	return vec;
}

void Animation::ReadMissingBones(NodeData& parentNode)
{
	if (parentNode.Type == UNKNOWN)
		return;

	const auto minId = BoneInfoMap.find(parentNode.BoneName)->second.Id;

	const auto nextType = static_cast<BoneType>(parentNode.Type + 1);
	auto maxId = -1;
	if (nextType != UNKNOWN)
		maxId = BoneInfoMap.find(Config::BoneTypeNames.at(nextType))->second.Id;
	else
		maxId = static_cast<int>(BoneInfoMap.size()) - 1;

	auto vec = GetBones(minId, maxId);
	parentNode.Children.insert(parentNode.Children.end(), vec.begin(), vec.end());
	parentNode.ChildrenCount += static_cast<int>(vec.size());

	for (auto& children : parentNode.Children)
		ReadMissingBones(children);
}