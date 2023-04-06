#include "Bone.hpp"

float Bone::GetScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime) const
{
	const auto midWayLength = animationTime - lastTimeStamp;
	const auto framesDiff = nextTimeStamp - lastTimeStamp;
	const auto scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

int Bone::GetPositionIndex(const float animationTime) const
{
	for (int index = 0; index < NumPositions - 1; ++index)
	{
		if (animationTime < Positions[index + 1].TimeStamp)
			return index;
	}
	throw std::exception("Animation error");
}

int Bone::GetRotationIndex(const float animationTime) const
{
	for (int index = 0; index < NumRotations - 1; ++index)
	{
		if (animationTime < Rotations[index + 1].TimeStamp)
			return index;
	}
	throw std::exception("Animation error");
}

int Bone::GetScaleIndex(const float animationTime) const
{
	for (int index = 0; index < NumScalings - 1; ++index)
	{
		if (animationTime < Scales[index + 1].TimeStamp)
			return index;
	}
	throw std::exception("Animation error");
}

glm::mat4 Bone::InterpolatePosition(const float animationTime) const
{
	if (NumPositions == 1)
		return glm::translate(glm::mat4(1.0f), Positions[0].Position);

	const auto bone0Index = GetPositionIndex(animationTime);
	const auto bone1Index = bone0Index + 1;
	const auto scaleFactor = GetScaleFactor(Positions[bone0Index].TimeStamp, Positions[bone1Index].TimeStamp, animationTime);

	const auto finalPosition = glm::mix(Positions[bone0Index].Position, Positions[bone1Index].Position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(const float animationTime) const
{
	if (NumRotations == 1)
	{
		const auto rotation = glm::normalize(Rotations[0].Orientation);
		return glm::toMat4(rotation);
	}

	const auto bone0Index = GetRotationIndex(animationTime);
	const auto bone1Index = bone0Index + 1;
	const auto scaleFactor = GetScaleFactor(Rotations[bone0Index].TimeStamp, Rotations[bone1Index].TimeStamp, animationTime);

	glm::quat finalRotation = glm::slerp(Rotations[bone0Index].Orientation, Rotations[bone1Index].Orientation	, scaleFactor);

	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(const float animationTime) const
{
	if (NumScalings == 1)
		return glm::scale(glm::mat4(1.0f), Scales[0].Scale);

	const auto bone0Index = GetScaleIndex(animationTime);
	const auto bone1Index = bone0Index + 1;
	const auto scaleFactor = GetScaleFactor(Scales[bone0Index].TimeStamp, Scales[bone1Index].TimeStamp, animationTime);

	const auto finalScale = glm::mix(Scales[bone0Index].Scale, Scales[bone1Index].Scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}

void Bone::Update(const float animationTime)
{
	const auto translation = InterpolatePosition(animationTime);
	const auto rotation = InterpolateRotation(animationTime);
	const auto scale = InterpolateScaling(animationTime);

	LocalTransform = translation * rotation * scale;
}