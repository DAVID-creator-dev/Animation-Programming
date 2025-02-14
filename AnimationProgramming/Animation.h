#pragma once
#include "math.h"
#include <vector>
#include <algorithm>
#include "Engine.h"

#define BONECOUNT 64

struct BoneTransform
{
    BoneTransform() : pos(Vec3(0, 0, 0)), rot(Quaternion(0, 0, 0, 0)) {}
    Vec3 pos;
    Quaternion rot;
    Mat4 mat;
};

class Animation
{
public:
    int keyFrame = 0;
    int currentKeyFrame = 0;

    const char* filename;

    std::vector<std::vector<BoneTransform>> animeSeq;
    std::vector<std::vector<BoneTransform>> globalTransforms;
    std::vector<BoneTransform> bindPoseTransforms;

    std::vector<int> parent; 

    Animation() = default;
    Animation(const char* _filename);

    void LoadAnimation();
private:
    void CombineTransform(const BoneTransform& parent, BoneTransform& children);
    void CombineTransform(const BoneTransform& parent, BoneTransform& children, BoneTransform& animation); 
};
