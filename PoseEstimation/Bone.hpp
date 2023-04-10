#pragma once

enum BoneType
{
    HEAD = 0,
    NECK = 1,
    LEFT_ARM = 2,
    LEFT_FOREARM = 3,
    LEFT_HAND = 4,
    RIGHT_ARM = 5,
    RIGHT_FOREARM = 6,
    RIGHT_HAND = 7,
    LEFT_UP_LEG = 8,
    LEFT_LEG = 9,
    LEFT_FOOT = 10,
    RIGHT_UP_LEG = 11,
    RIGHT_LEG = 12,
    RIGHT_FOOT = 13,
    UNKNOWN = 14,
};

struct BoneInfo
{
    // id is index in finalBoneMatrices
    int Id;

    // offset matrix transforms vertex from model space to bone space
    glm::mat4 Offset;
};