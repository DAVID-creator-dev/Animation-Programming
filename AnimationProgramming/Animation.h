#pragma once
#include "math.h"
#include <vector>

struct BoneTransform
{
    BoneTransform() : name(nullptr), pos(Vec3(0, 0, 0)), rot(Quaternion(0, 0, 0, 0)) {}
    const char* name;
    Vec3 pos;
    Quaternion rot;
};

class Animation
{
public:
    float currentTime = 0.0f;
    int currentKeyFrame = 0;

    std::vector<std::vector<BoneTransform>> animeSeq;
    std::vector<std::vector<BoneTransform>> globalTransforms;

    Animation() {}
    Animation(int keyFrame)
    {
        animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);
        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);
    }
    ~Animation() {}

    void PlayAnimation(float frameTime, int keyFrame, int boneCount, float blendSpeed)
    {
        frameTime *= blendSpeed;
        float animationDuration = keyFrame - 1;

        currentTime += frameTime;
        if (currentTime >= animationDuration)
            currentTime = 0.0f;

        currentKeyFrame = currentTime;
        int nextKeyFrame = currentKeyFrame + 1;

        if (nextKeyFrame >= keyFrame)
            nextKeyFrame = 0;

        float t = currentTime - currentKeyFrame;

        for (int j = 0; j < boneCount; j++)
        {
            int parentIndex = GetSkeletonBoneParentIndex(j);
            if (parentIndex != -1)
            {
                Vec3 posA = globalTransforms[currentKeyFrame][j].pos;
                Vec3 posB = globalTransforms[nextKeyFrame][j].pos;
                Vec3 parentPosA = globalTransforms[currentKeyFrame][parentIndex].pos;
                Vec3 parentPosB = globalTransforms[nextKeyFrame][parentIndex].pos;

                Vec3 interpolatedPos = posA.Lerp(posA, posB, t);
                Vec3 interpolatedParentPos = parentPosA.Lerp(parentPosA, parentPosB, t);

                DrawLine(interpolatedParentPos.x, interpolatedParentPos.y - 50, interpolatedParentPos.z,
                    interpolatedPos.x, interpolatedPos.y - 50, interpolatedPos.z,
                    1, 0, 0);
            }
        }
    }
};