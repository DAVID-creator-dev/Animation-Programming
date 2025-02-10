#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Engine.h"
#include "Simulation.h"
#include "math.h"

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


class CSimulation : public ISimulation
{
    int boneCount;
    int keyFrame;
    std::vector<std::vector<BoneTransform>> globalTransforms;
    std::vector<std::vector<BoneTransform>> animeSeq;
    float currentTime = 0.0f;
    int currentKeyFrame = 0;
    float speed = 10.0f;

    virtual void Init() override
    {
        boneCount = GetSkeletonBoneCount();
        keyFrame = GetAnimKeyCount("ThirdPersonWalk.anim");
        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);
        animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);


        for (int i = 0; i < keyFrame; i++) {
            globalTransforms[i] = std::vector<BoneTransform>(boneCount);
            animeSeq[i] = std::vector<BoneTransform>(boneCount);
            for (int j = 0; j < boneCount; j++)
            {
                GetSkeletonBoneLocalBindTransform(j,
                    globalTransforms[i][j].pos.x, globalTransforms[i][j].pos.y, globalTransforms[i][j].pos.z,
                    globalTransforms[i][j].rot.w, globalTransforms[i][j].rot.x, globalTransforms[i][j].rot.y, globalTransforms[i][j].rot.z
                );

                GetAnimLocalBoneTransform("ThirdPersonWalk.anim", j, i, animeSeq[i][j].pos.x, animeSeq[i][j].pos.y, animeSeq[i][j].pos.z, animeSeq[i][j].rot.w, animeSeq[i][j].rot.x, animeSeq[i][j].rot.y, animeSeq[i][j].rot.z);

                int parentIndex = GetSkeletonBoneParentIndex(j);
                if (parentIndex != -1)
                    CombineTransform(globalTransforms[i][parentIndex], globalTransforms[i][j], animeSeq[i][j]);
            }
        }
    }

    virtual void Update(float frameTime) override
    {
        frameTime *= speed;
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

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}
