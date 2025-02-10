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

void CombineTransform(const BoneTransform& parent, BoneTransform& children, BoneTransform& animation)
{
    Vec3 globalPos = children.pos + animation.pos;

    Quaternion globalRot = children.rot * animation.rot;

    Vec3 rotatedPos = parent.rot.multiplyVector(globalPos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * globalRot;
}

class Animation
{
public:
    int keyFrame;
    const char* filename;
    float currentTime = 0.0f;
    int currentKeyFrame = 0;
    int animationDuration;
    std::vector<std::vector<BoneTransform>> animeSeq;
    std::vector<std::vector<BoneTransform>> globalTransforms;

    Animation() {}
    Animation(const char* _filename)
    {
        filename = _filename;
    }
    ~Animation() {}

    void PlayAnimation(float frameTime, int boneCount, float blendSpeed)
    {
        frameTime *= blendSpeed;
        animationDuration = keyFrame - 1;

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

    void LoadAnimation(int boneCount)
    {
        keyFrame = GetAnimKeyCount(filename);

        animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);
        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);

        for (int i = 0; i < keyFrame; i++)
        {
            animeSeq[i] = std::vector<BoneTransform>(boneCount);
            globalTransforms[i] = std::vector<BoneTransform>(boneCount);

            for (int j = 0; j < boneCount; j++)
            {
                GetSkeletonBoneLocalBindTransform(j, globalTransforms[i][j].pos.x, globalTransforms[i][j].pos.y, globalTransforms[i][j].pos.z,
                    globalTransforms[i][j].rot.w, globalTransforms[i][j].rot.x, globalTransforms[i][j].rot.y, globalTransforms[i][j].rot.z);

                GetAnimLocalBoneTransform(filename, j, i, animeSeq[i][j].pos.x, animeSeq[i][j].pos.y, animeSeq[i][j].pos.z, animeSeq[i][j].rot.w, animeSeq[i][j].rot.x, animeSeq[i][j].rot.y, animeSeq[i][j].rot.z);

                int parentIndex = GetSkeletonBoneParentIndex(j);
                if (parentIndex != -1)
                    CombineTransform(globalTransforms[i][parentIndex], globalTransforms[i][j], animeSeq[i][j]);
            }
        }
    }

    /*float GetDuration(Animation& anim)
    {
        return animationDuration / anim.animationDuration;
    }*/
};