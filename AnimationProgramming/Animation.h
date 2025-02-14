#pragma once
#include "math.h"
#include <vector>
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

class Blend {
public:
    std::vector<float> skinningData;
    
    std::vector<Animation*> animations;
    Animation* currentAnimation;
    Animation* nextAnimation;
    float currentTime = 0.0f;

    float blendFactor = 0.f;
    float blendDuration = 0.2f;  
    float blendTimeElapsed = 0.0f;

    Blend()
    {
        skinningData = std::vector<float>(BONECOUNT * 16);
    }

    void SetCurrentAnimation(Animation* animation)
    {
        currentAnimation = animation;
    }
    
    void AddAnimation(Animation* animation)
    {
        animations.push_back(animation);
    }

    void SetNextAnimation(Animation* newAnimation, float blendDuration)
    {
        if (currentAnimation == newAnimation) return;  

        nextAnimation = newAnimation;
        blendDuration = blendDuration;
        blendTimeElapsed = 0.0f;
        blendFactor = 0.0f;
    }

    void PlayAnimation(float frameTime, float blendSpeed)
    {
        if (!currentAnimation) return;

        int keyFrame = currentAnimation->keyFrame;
        float adjustedFrameTime = frameTime * blendSpeed;
        float animationDuration = keyFrame - 1;

        if (!nextAnimation || blendFactor >= 1.0f)
        {
            currentTime += adjustedFrameTime;
            if (currentTime >= animationDuration)
                currentTime = 0.0f;
        }

        int currentKeyFrame = static_cast<int>(currentTime);
        int nextKeyFrame = (currentKeyFrame + 1) % keyFrame;
        float t = currentTime - currentKeyFrame;

        for (int j = 0; j < BONECOUNT; j++)
        {
            BoneTransform interpolatedBone;

            if (nextAnimation)
            {
                int nextAnimKeyFrame = currentKeyFrame;
                int nextAnimNextKeyFrame = (nextAnimKeyFrame + 1) % nextAnimation->keyFrame;

                Vec3 pos1 = currentAnimation->globalTransforms[currentKeyFrame][j].pos.Lerp(
                    currentAnimation->globalTransforms[currentKeyFrame][j].pos,
                    currentAnimation->globalTransforms[nextKeyFrame][j].pos, t
                );

                Quaternion rot1 = currentAnimation->globalTransforms[currentKeyFrame][j].rot.Slerp(
                    currentAnimation->globalTransforms[currentKeyFrame][j].rot,
                    currentAnimation->globalTransforms[nextKeyFrame][j].rot, t
                );

                Vec3 pos2 = nextAnimation->globalTransforms[nextAnimKeyFrame][j].pos.Lerp(
                    nextAnimation->globalTransforms[nextAnimKeyFrame][j].pos,
                    nextAnimation->globalTransforms[nextAnimNextKeyFrame][j].pos, t
                );

                Quaternion rot2 = nextAnimation->globalTransforms[nextAnimKeyFrame][j].rot.Slerp(
                    nextAnimation->globalTransforms[nextAnimKeyFrame][j].rot,
                    nextAnimation->globalTransforms[nextAnimNextKeyFrame][j].rot, t
                );

                interpolatedBone.pos = pos1.Lerp(pos1, pos2, blendFactor);
                interpolatedBone.rot = rot1.Slerp(rot1, rot2, blendFactor);
            }
            else
            {
                interpolatedBone.pos = currentAnimation->globalTransforms[currentKeyFrame][j].pos.Lerp(
                    currentAnimation->globalTransforms[currentKeyFrame][j].pos,
                    currentAnimation->globalTransforms[nextKeyFrame][j].pos, t
                );

                interpolatedBone.rot = currentAnimation->globalTransforms[currentKeyFrame][j].rot.Slerp(
                    currentAnimation->globalTransforms[currentKeyFrame][j].rot,
                    currentAnimation->globalTransforms[nextKeyFrame][j].rot, t
                );
            }

            interpolatedBone.mat.TRS(interpolatedBone.pos, interpolatedBone.rot);
            interpolatedBone.mat = interpolatedBone.mat * currentAnimation->bindPoseTransforms[j].mat.InvertMatrix();
            interpolatedBone.mat.TransposeMatrix();

            std::memcpy(&skinningData[j * 16], interpolatedBone.mat.data, 16 * sizeof(float));
        }

        if (nextAnimation)
        {
            blendTimeElapsed += frameTime;
            blendFactor = blendTimeElapsed / blendDuration;

            if (blendFactor >= 1.0f)
            {
                currentAnimation = nextAnimation;
                nextAnimation = nullptr;
                blendFactor = 0.0f;
                blendTimeElapsed = 0.0f;
            }
        }

        SetSkinningPose(skinningData.data(), 64);
    }
}; 
