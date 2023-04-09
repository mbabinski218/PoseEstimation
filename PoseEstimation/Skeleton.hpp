#pragma once
#include "Bone.hpp"

struct Skeleton
{
	std::map<BoneType, cv::Point2f> Current{};
	std::map<BoneType, cv::Point2f> Previous{};

	void MoveAndClear()
	{
		Previous = std::move(Current);
		Current.clear();
	}

	std::map<BoneType, glm::vec3> GetBonesTransformations() const
	{
		auto map = std::map<BoneType, glm::vec3>{};

		for(const auto& point : Current)
		{			
			if(Previous.contains(point.first))
			{
				const auto prev = Previous.at(point.first);
				map.try_emplace(point.first, glm::vec3{point.second.x - prev.x, point.second.y - prev.y, 0});
			}
			else
			{
				map.try_emplace(point.first, glm::vec3{point.second.x, point.second.y, 0});
			}
		}

		return map;
	}
};