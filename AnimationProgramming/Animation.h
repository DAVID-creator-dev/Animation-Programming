#pragma once
#include "math.h"
#include <vector>
#include "Engine.h"

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
    const char* filename;
    float currentTime = 0.0f;
    int currentKeyFrame = 0;
    int animationDuration;
    int nextKeyFrame;
    std::vector<std::vector<BoneTransform>> animeSeq;
    std::vector<std::vector<BoneTransform>> globalTransforms;
    std::vector<BoneTransform> bindPoseTransforms;
    std::vector<float> skinningData;

    Animation() {}
    Animation(const char* _filename)
    {
        filename = _filename;
    }
    ~Animation() {}

    void PlayAnimation(float frameTime, int boneCount, float blendSpeed)
    {
        float animationDuration = (keyFrame - 1);
        currentTime += frameTime * blendSpeed;

        if (currentTime >= animationDuration)
        {
            currentTime = 0.0f;
        }
          
        currentKeyFrame = currentTime;
        nextKeyFrame = currentKeyFrame + 1;

        if (nextKeyFrame >= keyFrame)
        {
           nextKeyFrame = 0;
        }

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

    void LoadAnimation(int boneCount)
    {
        keyFrame = GetAnimKeyCount(filename);

        animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);
        globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);
        bindPoseTransforms = std::vector<BoneTransform>(boneCount);
        skinningData = std::vector<float>(boneCount * 16);

        for (int i = 0; i < keyFrame; i++) 
        {
            animeSeq[i] = std::vector<BoneTransform>(boneCount);
            globalTransforms[i] = std::vector<BoneTransform>(boneCount);

            for (int j = 0; j < boneCount; j++)
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

    void BlendAnimation(Animation* anim2, int boneCount)
    {
        float blendFactor = 0;
        std::vector<BoneTransform> blendedTransform(boneCount);
        blendFactor = Vec3::Clamp(blendFactor, 0.1f, 1.0f);
        std::vector<float> blendSkinningData;;
        
        for (int i = 0; i < boneCount; ++i)
        {
            blendedTransform[i].pos = Vec3::Lerp(globalTransforms[currentKeyFrame][i].pos, anim2->globalTransforms[nextKeyFrame][i].pos, blendFactor);
            blendedTransform[i].rot = Quaternion::Slerp(globalTransforms[currentKeyFrame][i].rot, anim2->globalTransforms[nextKeyFrame][i].rot, blendFactor);

            blendedTransform[i].mat.TRS(blendedTransform[i].pos, blendedTransform[i].rot);
            blendedTransform[i].mat = blendedTransform[i].mat * blendedTransform[i].mat.InvertMatrix();
            blendedTransform[i].mat.TransposeMatrix();

           // std::memcpy(&blendSkinningData[i * 16], blendedTransform[i].mat.data, 16 * sizeof(float));
        }
       // SetSkinningPose(blendSkinningData.data(), 64);
      //  return;
    }
};