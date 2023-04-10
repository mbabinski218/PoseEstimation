#pragma once
#include "Bone.hpp"
#include "Skeleton.hpp"

struct NodeData
{
	std::string BoneName;
	BoneType Type;
	int ChildrenCount;
	std::vector<NodeData> Children;
	glm::mat4 Transformation;
};

class Animation
{
public:
	Animation(const Skeleton& skeleton, const std::unordered_map<std::string, BoneInfo>& boneInfoMap);
	[[nodiscard]] const NodeData& GetRootNode() { return RootNode; }
	[[nodiscard]] const std::unordered_map<std::string, BoneInfo>& GetBoneIdMap() { return BoneInfoMap; }

private:
	void Fill(BoneType boneType, NodeData& parentNode, const std::map<BoneType, glm::vec3>& transformations);
	[[nodiscard]] std::vector<NodeData> GetBones(int minId, int maxId);
	void ReadMissingBones(NodeData& parentNode, const std::map<BoneType, glm::vec3>& transformations);

	NodeData RootNode{};
	std::unordered_map<std::string, BoneInfo> BoneInfoMap{};
};