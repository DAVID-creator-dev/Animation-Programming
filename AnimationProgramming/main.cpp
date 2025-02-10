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
    Mat4 mat;
};

void CombineTransform(const BoneTransform& parent, BoneTransform& children)
{
    Vec3 rotatedPos = parent.rot.multiplyVector(children.pos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * children.rot;

    children.mat.TRS(children.pos, children.rot);
}

void CombineTransform(const BoneTransform& parent, BoneTransform& children, BoneTransform& animation)
{
    Vec3 globalPos = children.pos + animation.pos;

    Quaternion globalRot = children.rot * animation.rot;

    Vec3 rotatedPos = parent.rot.multiplyVector(globalPos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * globalRot;

    children.mat.TRS(children.pos, children.rot);
}

class CSimulation : public ISimulation
{
    int boneCount;
    int keyFrame;
    std::vector<std::vector<BoneTransform>> globalTransforms;
    std::vector<std::vector<BoneTransform>> animeTransforms;
    std::vector<BoneTransform> bindPoseTransforms;
    std::vector<float> skinningData;

    float currentTime = 0.0f;
    int currentKeyFrame = 0;
    float speed = 10.0f;

    virtual void Init() override
    {
        boneCount = 64;
        keyFrame = GetAnimKeyCount("ThirdPersonWalk.anim");

        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame, std::vector<BoneTransform>(boneCount));
        animeTransforms = std::vector<std::vector<BoneTransform>>(keyFrame, std::vector<BoneTransform>(boneCount));
        bindPoseTransforms = std::vector<BoneTransform>(boneCount);
        skinningData = std::vector<float>(boneCount * 16);

        for (int i = 0; i < keyFrame; i++) {
            for (int j = 0; j < boneCount; j++) {
                GetSkeletonBoneLocalBindTransform(j,
                    globalTransforms[i][j].pos.x, globalTransforms[i][j].pos.y, globalTransforms[i][j].pos.z,
                    globalTransforms[i][j].rot.w, globalTransforms[i][j].rot.x, globalTransforms[i][j].rot.y, globalTransforms[i][j].rot.z
                );

                GetAnimLocalBoneTransform("ThirdPersonWalk.anim", j, i, animeTransforms[i][j].pos.x, animeTransforms[i][j].pos.y, animeTransforms[i][j].pos.z,
                    animeTransforms[i][j].rot.w, animeTransforms[i][j].rot.x, animeTransforms[i][j].rot.y, animeTransforms[i][j].rot.z);

                bindPoseTransforms[j] = globalTransforms[i][j];

                int parentIndex = GetSkeletonBoneParentIndex(j);
                if (parentIndex != -1)
                {
                    CombineTransform(globalTransforms[i][parentIndex], globalTransforms[i][j], animeTransforms[i][j]);
                    CombineTransform(bindPoseTransforms[parentIndex], bindPoseTransforms[j]);
                }
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
            BoneTransform interpolatedBone;

            interpolatedBone.pos = globalTransforms[currentKeyFrame][j].pos.Lerp(
                globalTransforms[currentKeyFrame][j].pos,
                globalTransforms[nextKeyFrame][j].pos, t
            );

            interpolatedBone.rot = globalTransforms[currentKeyFrame][j].rot.Slerp(
                globalTransforms[currentKeyFrame][j].rot,
                globalTransforms[nextKeyFrame][j].rot, t
            );

            interpolatedBone.mat.TRS(interpolatedBone.pos, interpolatedBone.rot);
            interpolatedBone.mat = interpolatedBone.mat * bindPoseTransforms[j].mat.InvertMatrix(); 
            interpolatedBone.mat.TransposeMatrix(); 

            std::memcpy(&skinningData[j * 16], interpolatedBone.mat.data, 16 * sizeof(float));

            
            int parentIndex = GetSkeletonBoneParentIndex(j);
            if (parentIndex != -1)
            {
                Vec3 interpolatedParentPos = globalTransforms[currentKeyFrame][parentIndex].pos.Lerp(
                    globalTransforms[currentKeyFrame][parentIndex].pos,
                    globalTransforms[nextKeyFrame][parentIndex].pos, t
                );

                DrawLine(interpolatedParentPos.x, interpolatedParentPos.y - 50, interpolatedParentPos.z,
                    interpolatedBone.pos.x, interpolatedBone.pos.y - 50, interpolatedBone.pos.z,
                    1, 0, 0);
            }

        }

        SetSkinningPose(skinningData.data(), 64);
    }

};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}
