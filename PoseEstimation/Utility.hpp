#pragma once
#include <ranges>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include "Bone.hpp"

enum DnnTargetMode
{
	CPU,
	CUDA
};

template<typename T>
struct CameraLinker
{
	T Link;
};

class Converter
{
public:

	static cv::Size2f ToSize2f(const ImVec2& vec)
	{
		return {vec.x, vec.y};
	}

	static glm::mat4 ToGlmMatrix(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static glm::vec3 ToGlmVec3(const aiVector3D& vec)
	{
		return {vec.x, vec.y, vec.z};
	}

	static glm::quat ToGlmQuat(const aiQuaternion& quat)
	{
		return {quat.w, quat.x, quat.y, quat.z};
	}

	static std::string ToString(const BoneType boneType)
	{
		switch(boneType)
		{
		case HEAD:			return "Head";
		case NECK:			return "Neck";
		case RIGHT_ARM:		return "RightArm";
		case RIGHT_FOREARM: return "RightForeArm";
		case RIGHT_HAND:	return "RightHand";
		case LEFT_ARM:		return "LeftArm";
		case LEFT_FOREARM:  return "LeftForeArm";
		case LEFT_HAND:		return "LeftHand";
		case RIGHT_UP_LEG:  return "RightUpLeg";
		case RIGHT_LEG:		return "RightLeg";
		case RIGHT_FOOT:	return "RightFoot";
		case LEFT_UP_LEG:	return "LeftUpLeg";
		case LEFT_LEG:		return "LeftLeg";
		case LEFT_FOOT:		return "LeftFoot";
		default:			throw std::exception("Bone mapping error");
		}		
	}
};

static auto FindBetween(const std::unordered_map<std::string, BoneInfo>& map, const int minIndex, const int maxIndex)
{
	//const std::unordered_map<std::string, BoneInfo> subrange;
	//std::copy_if(map.begin(), map.end(), subrange, [&](const std::pair<std::string, BoneInfo>& boneInfo)
	//{
	//	return boneInfo.second.Id > minIndex && boneInfo.second.Id < maxIndex;
	//});

	/*return subrange;*/

	return map | std::ranges::views::filter([&](const std::pair<std::string, BoneInfo>& boneInfo)
	{
		return boneInfo.second.Id > minIndex && boneInfo.second.Id < maxIndex;
	});
}

template<typename T, typename U>
static bool IsMapContains(const std::map<T, U>& map, const T element)
{
	return std::ranges::any_of(map, [&](const std::pair<T, U>& mapElement)
	{
		return mapElement.first == element;
	});
}