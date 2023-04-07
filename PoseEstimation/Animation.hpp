#pragma once
#include "Libs.hpp"
#include "Bone.hpp"
#include "Skeleton.hpp"

struct NodeData
{
	glm::mat4 Transformation;
	std::string Name;
	int ChildrenCount;
	std::vector<NodeData> Children;
};

class Animation
{
public:
	[[nodiscard]] static std::shared_ptr<Animation> Create(const Skeleton& skeleton, const std::map<std::string, BoneInfo>& boneInfoMap, float duration = 1.0f, int ticksPerSecond = 1);

	[[nodiscard]] Bone* FindBone(const std::string& name);
	[[nodiscard]] int GetTicksPerSecond() const { return TicksPerSecond; }
	[[nodiscard]] float GetDuration() const { return Duration; }
	[[nodiscard]] const NodeData& GetRootNode() { return RootNode; }
	[[nodiscard]] const std::map<std::string, BoneInfo>& GetBoneIdMap()	{ return BoneInfoMap; }

private:
	float Duration = 0.0f;
	int TicksPerSecond = 0;
	NodeData RootNode{};
	std::vector<Bone> Bones{};
	std::map<std::string, BoneInfo> BoneInfoMap{};
};