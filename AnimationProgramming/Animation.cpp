#include "Animation.h"

Animation::Animation(const char* _filename)
{
    filename = _filename;

    keyFrame = GetAnimKeyCount(filename);
    
    animeSeq = std::vector<std::vector<BoneTransform>>(keyFrame);
    globalTransforms = std::vector<std::vector<BoneTransform>>(keyFrame);
    bindPoseTransforms = std::vector<BoneTransform>(BONECOUNT);
    parent = std::vector<int>(BONECOUNT); 
}

void Animation::CombineTransform(const BoneTransform& parent, BoneTransform& children)
{
    Vec3 rotatedPos = parent.rot.multiplyVector(children.pos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * children.rot;

    children.mat.TRS(children.pos, children.rot);
}

void Animation::CombineTransform(const BoneTransform& parent, BoneTransform& children, BoneTransform& animation)
{
    Vec3 globalPos = children.pos + animation.pos;

    Quaternion globalRot = children.rot * animation.rot;

    Vec3 rotatedPos = parent.rot.multiplyVector(globalPos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * globalRot;

    children.mat.TRS(children.pos, children.rot);
}

void Animation::LoadAnimation()
{
    for (int i = 0; i < keyFrame; i++)
    {
        animeSeq[i] = std::vector<BoneTransform>(BONECOUNT);
        globalTransforms[i] = std::vector<BoneTransform>(BONECOUNT);

        for (int j = 0; j < BONECOUNT; j++)
        {
            parent[j] = GetSkeletonBoneParentIndex(j); 

            GetSkeletonBoneLocalBindTransform(j,
                globalTransforms[i][j].pos.x, globalTransforms[i][j].pos.y, globalTransforms[i][j].pos.z,
                globalTransforms[i][j].rot.w, globalTransforms[i][j].rot.x, globalTransforms[i][j].rot.y, globalTransforms[i][j].rot.z
            );

            GetAnimLocalBoneTransform(filename, j, i, 
                animeSeq[i][j].pos.x, animeSeq[i][j].pos.y, animeSeq[i][j].pos.z,
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
