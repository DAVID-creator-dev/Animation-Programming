#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <iostream>
#include "Engine.h"
#include "Simulation.h"
#include "Animation.h"

class CSimulation : public ISimulation
{
    int boneCount;
    int keyFrame;

    std::vector<Animation*> anims;
    Animation* walkAnimation;
    Animation* runAnimation;

    void CombineTransform(const BoneTransform& parent, BoneTransform& children, BoneTransform& animation)
    {
        Vec3 globalPos = children.pos + animation.pos;

        Quaternion globalRot = children.rot * animation.rot;

        Vec3 rotatedPos = parent.rot.multiplyVector(globalPos);

        children.pos = parent.pos + rotatedPos;

        children.rot = parent.rot * globalRot;
    }

    virtual void Init() override
    {
        boneCount = GetSkeletonBoneCount();
        keyFrame = GetAnimKeyCount("ThirdPersonWalk.anim");

        walkAnimation = new Animation(keyFrame);
        runAnimation = new Animation(keyFrame);

        anims.push_back(walkAnimation);
        anims.push_back(runAnimation);

        for (int k = 0; k < anims.size(); ++k)
        {
            for (int i = 0; i < keyFrame; i++)
            {
                anims[k]->animeSeq[i] = std::vector<BoneTransform>(boneCount);
                anims[k]->globalTransforms[i] = std::vector<BoneTransform>(boneCount);

                for (int j = 0; j < boneCount; j++)
                {
                    GetSkeletonBoneLocalBindTransform(j, anims[k]->globalTransforms[i][j].pos.x, anims[k]->globalTransforms[i][j].pos.y, anims[k]->globalTransforms[i][j].pos.z,
                        anims[k]->globalTransforms[i][j].rot.w, anims[k]->globalTransforms[i][j].rot.x, anims[k]->globalTransforms[i][j].rot.y, anims[k]->globalTransforms[i][j].rot.z);

                    GetAnimLocalBoneTransform("ThirdPersonWalk.anim", j, i, anims[k]->animeSeq[i][j].pos.x, anims[k]->animeSeq[i][j].pos.y, anims[k]->animeSeq[i][j].pos.z, anims[k]->animeSeq[i][j].rot.w, anims[k]->animeSeq[i][j].rot.x, anims[k]->animeSeq[i][j].rot.y, anims[k]->animeSeq[i][j].rot.z);

                    int parentIndex = GetSkeletonBoneParentIndex(j);
                    if (parentIndex != -1)
                        CombineTransform(anims[k]->globalTransforms[i][parentIndex], anims[k]->globalTransforms[i][j], anims[k]->animeSeq[i][j]);
                }
            }
        }
    }

    virtual void Update(float frameTime) override
    {
        runAnimation->PlayAnimation(frameTime, keyFrame, boneCount, 15.0f);
    }
};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}
