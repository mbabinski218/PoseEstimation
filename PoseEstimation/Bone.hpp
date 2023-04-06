#pragma once
#include "Libs.hpp"

enum BoneType
{
    HEAD = 0,
    NECK = 1,
    RIGHT_ARM = 2,
    RIGHT_FOREARM = 3,
    RIGHT_HAND = 4,
    LEFT_ARM = 5,
    LEFT_FOREARM = 6,
    LEFT_HAND = 7,
    RIGHT_UP_LEG = 8,
    RIGHT_LEG = 9,
    RIGHT_FOOT = 10,
    LEFT_UP_LEG = 11,
    LEFT_LEG = 12,
    LEFT_FOOT = 13
};

struct BoneInfo
{
    // id is index in finalBoneMatrices
    int Id;

    // offset matrix transforms vertex from model space to bone space
    glm::mat4 Offset;
};

struct KeyPosition
{
    glm::vec3 Position;
    float TimeStamp;
};

struct KeyRotation
{
    glm::quat Orientation;
    float TimeStamp;
};

struct KeyScale
{
    glm::vec3 Scale;
    float TimeStamp;
};

class Bone
{
public:
    explicit Bone(std::string name, const int id) : LocalTransform(1.0f), Name(std::move(name)), Id(id) {}

    void Update(float animationTime);
    [[nodiscard]] std::string GetName() const { return Name; }
    [[nodiscard]] glm::mat4 GetLocalTransform() const { return LocalTransform; }

    void AddPosition(const glm::vec3& position, float timeStamp) { Positions.emplace_back(position, timeStamp); NumPositions++; }
    void AddRotation(const glm::quat& orientation, float timeStamp) { Rotations.emplace_back(orientation, timeStamp); NumRotations++; }
    void AddScale(const glm::vec3& scale, float timeStamp) { Scales.emplace_back(scale, timeStamp); NumScalings++; }

private:
    [[nodiscard]] float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const;
    [[nodiscard]] int GetPositionIndex(float animationTime) const;
    [[nodiscard]] int GetRotationIndex(float animationTime) const;
    [[nodiscard]] int GetScaleIndex(float animationTime) const;
    [[nodiscard]] glm::mat4 InterpolatePosition(float animationTime) const;
    [[nodiscard]] glm::mat4 InterpolateRotation(float animationTime) const;
    [[nodiscard]] glm::mat4 InterpolateScaling(float animationTime) const;

    std::vector<KeyPosition> Positions{};
    std::vector<KeyRotation> Rotations{};
    std::vector<KeyScale> Scales{};
    int NumPositions = 0;
    int NumRotations = 0;
    int NumScalings = 0;

    glm::mat4 LocalTransform{};
    std::string Name{};
    int Id = -1;
};