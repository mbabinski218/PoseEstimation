#pragma once
#include "Libs.hpp"

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int Id;

    /*offset matrix transforms vertex from model space to bone space*/
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
    std::vector<KeyPosition> Positions;
    std::vector<KeyRotation> Rotations;
    std::vector<KeyScale> Scales;
    int NumPositions;
    int NumRotations;
    int NumScalings;

    glm::mat4 LocalTransform;
    std::string Name;
    int Id;

public:


};