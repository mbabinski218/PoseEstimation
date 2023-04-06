#include "Animation.hpp"
#include "Utility.hpp"

std::shared_ptr<Animation> Animation::Create(const std::map<std::string, BoneInfo>& boneInfoMap, const float duration, const int ticksPerSecond)
{
	Animation animation{};
	animation.BoneInfoMap = boneInfoMap;
	animation.Duration = duration;
	animation.TicksPerSecond = ticksPerSecond;

	NodeData node3{};
	node3.Name = Converter::ToString(LEFT_HAND);
	node3.ChildrenCount = 0;
	node3.Transformation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.3f));

	NodeData node2{};
	node2.Name = Converter::ToString(LEFT_FOREARM);
	node2.ChildrenCount = 1;
	node2.Children.push_back(node3);
	node2.Transformation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.3f));

	NodeData node1{};
	node1.Name = Converter::ToString(LEFT_ARM);
	node1.ChildrenCount = 2;
	node1.Children.push_back(node2);
	node1.Children.push_back(node3);
	node1.Transformation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.3f));
		
	animation.RootNode = node1;

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
