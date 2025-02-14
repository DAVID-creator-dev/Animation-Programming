#pragma once
#include "math.h"
#include <vector>
#include <algorithm>
#include "Engine.h"

#define BONECOUNT 64

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

class Animation
{
public:
    int keyFrame;
    int currentKeyFrame = 0;
    const char* filename;
    int animationDuration;
    std::vector<std::vector<BoneTransform>> animeSeq;
    std::vector<std::vector<BoneTransform>> globalTransforms;
    std::vector<BoneTransform> bindPoseTransforms;

    Animation() {}
    Animation(const char* _filename)
    {
        filename = _filename;
    }
    ~Animation() {}

    void LoadAnimation()
    {
        keyFrame = GetAnimKeyCount(filename);

        animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);
        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);
        bindPoseTransforms = std::vector<BoneTransform>(BONECOUNT);

        for (int i = 0; i < keyFrame; i++) 
        {
            animeSeq[i] = std::vector<BoneTransform>(BONECOUNT);
            globalTransforms[i] = std::vector<BoneTransform>(BONECOUNT);

            for (int j = 0; j < BONECOUNT; j++)
            {
                GetSkeletonBoneLocalBindTransform(j,
                    globalTransforms[i][j].pos.x, globalTransforms[i][j].pos.y, globalTransforms[i][j].pos.z,
                    globalTransforms[i][j].rot.w, globalTransforms[i][j].rot.x, globalTransforms[i][j].rot.y, globalTransforms[i][j].rot.z
                );

                GetAnimLocalBoneTransform(filename, j, i, animeSeq[i][j].pos.x, animeSeq[i][j].pos.y, animeSeq[i][j].pos.z,
                    animeSeq[i][j].rot.w, animeSeq[i][j].rot.x, animeSeq[i][j].rot.y, animeSeq[i][j].rot.z);

                bindPoseTransforms[j] = globalTransforms[i][j];

                int parentIndex = GetSkeletonBoneParentIndex(j);
                if (parentIndex != -1)
                {
                    CombineTransform(globalTransforms[i][parentIndex], globalTransforms[i][j], animeSeq[i][j]);
                    CombineTransform(bindPoseTransforms[parentIndex], bindPoseTransforms[j]);
                }
            }
        }
    }
};

class Blend
{
public:
    std::vector<float> skinningData;
    float currentTime = 0.0f;
    bool isPaused = false;
    int keyFrame = 0;
    float adjustedFrameTime = 0.0f;
    float animationDuration = 0.0f;
    int currentKeyFrame = 0;
    int nextKeyFrame = 0;
    float t = 0.0f;
    Vec3 interpolatedParentPos = Vec3(0, 0, 0);
    bool isRewind = false;

    Blend()
    {
        skinningData = std::vector<float>(BONECOUNT * 16);
    }

    void ChangeBlendDirection()
    {
        isRewind = !isRewind;
    }

    void ChangeAnimState()
    {
        isPaused = !isPaused;
    }

    void DrawAnimation(BoneTransform interpolatedBone) {

    }

    void PlayAnimation(float frameTime, float blendSpeed, float blendFactor, Animation* anim1, Animation* anim2)
    {
        if (!isPaused)
        {
            keyFrame = anim1->keyFrame;
            adjustedFrameTime = frameTime * blendSpeed;
            animationDuration = keyFrame - 1;

            currentTime += adjustedFrameTime;
            if (currentTime >= animationDuration)
                currentTime = 0.0f;

            if (isRewind)
            {
                currentKeyFrame = keyFrame - 1 - static_cast<int>(currentTime);
                nextKeyFrame = (currentKeyFrame - 1) % keyFrame;
                t = currentTime - currentKeyFrame - 1 - keyFrame;
            }
            else
            {
                currentKeyFrame =static_cast<int>(currentTime);
                nextKeyFrame = (currentKeyFrame + 1) % keyFrame;
                t = currentTime - currentKeyFrame;
            }
        }
        

        for (int j = 0; j < BONECOUNT; j++)
        {
            BoneTransform interpolatedBone;
            BoneTransform interpolatedBoneParent;

            float frameRatio = static_cast<float>(anim2->keyFrame) / static_cast<float>(anim1->keyFrame);
            float nextAnimTime = currentTime * frameRatio;

            int nextAnimKeyFrame = static_cast<int>(nextAnimTime);
            int nextAnimNextKeyFrame = (nextAnimKeyFrame + 1) % anim2->keyFrame;
            float t2 = nextAnimTime - nextAnimKeyFrame;

            Vec3 pos1 = anim1->globalTransforms[currentKeyFrame][j].pos.Lerp(
                anim1->globalTransforms[currentKeyFrame][j].pos,
                anim1->globalTransforms[nextKeyFrame][j].pos, t
            );

            Quaternion rot1 = anim1->globalTransforms[currentKeyFrame][j].rot.Slerp(
                anim1->globalTransforms[currentKeyFrame][j].rot,
                anim1->globalTransforms[nextKeyFrame][j].rot, t
            );

            Vec3 pos2 = anim2->globalTransforms[nextAnimKeyFrame][j].pos.Lerp(
                anim2->globalTransforms[nextAnimKeyFrame][j].pos,
                anim2->globalTransforms[nextAnimNextKeyFrame][j].pos, t2
            );

            Quaternion rot2 = anim2->globalTransforms[nextAnimKeyFrame][j].rot.Slerp(
                anim2->globalTransforms[nextAnimKeyFrame][j].rot,
                anim2->globalTransforms[nextAnimNextKeyFrame][j].rot, t2
            );

            interpolatedBone.pos = pos1.Lerp(pos1, pos2, blendFactor);
            interpolatedBone.rot = rot1.Slerp(rot1, rot2, blendFactor);

            interpolatedBone.mat.TRS(interpolatedBone.pos, interpolatedBone.rot);
            interpolatedBone.mat = interpolatedBone.mat * anim1->bindPoseTransforms[j].mat.InvertMatrix();
            interpolatedBone.mat.TransposeMatrix();

            std::memcpy(&skinningData[j * 16], interpolatedBone.mat.data, 16 * sizeof(float));

            //Parent
            int parentIndex = GetSkeletonBoneParentIndex(j);
            if (parentIndex != -1)
            {

                Vec3 parentPos1 = anim1->globalTransforms[currentKeyFrame][parentIndex].pos.Lerp(
                    anim1->globalTransforms[currentKeyFrame][parentIndex].pos,
                    anim1->globalTransforms[nextKeyFrame][parentIndex].pos, t
                );

                Vec3 parentPos2 = anim2->globalTransforms[nextAnimKeyFrame][parentIndex].pos.Lerp(
                    anim2->globalTransforms[nextAnimKeyFrame][parentIndex].pos,
                    anim2->globalTransforms[nextAnimNextKeyFrame][parentIndex].pos, t2
                );

                interpolatedBoneParent.pos = parentPos1.Lerp(parentPos1, parentPos2, blendFactor);
                DrawLine(interpolatedBoneParent.pos.x - 80, interpolatedBoneParent.pos.y, interpolatedBoneParent.pos.z,
                    interpolatedBone.pos.x - 80, interpolatedBone.pos.y, interpolatedBone.pos.z,
                    1, 0, 0);
            }
        }

        SetSkinningPose(skinningData.data(), BONECOUNT);
    }
}; 
