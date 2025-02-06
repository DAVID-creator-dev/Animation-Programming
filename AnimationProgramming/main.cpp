// main.cpp : Defines the entry point for the console application.
//

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

class Animation
{
public:
    Animation() {}
    ~Animation() {}
    std::vector<std::vector<BoneTransform>> animSequence;
};


void CombineTransform(const BoneTransform& parent, BoneTransform& children) {
    Vec3 rotatedPos = parent.rot.multiplyVector(children.pos);

    children.pos = parent.pos + rotatedPos;

    children.rot = parent.rot * children.rot;
}


class CSimulation : public ISimulation
{
    int boneCount; 
    std::vector<BoneTransform> globalTransforms;
    Animation walkAnim = Animation();
    std::vector<BoneTransform> animSeque1;

    virtual void Init() override
    {
        boneCount = GetSkeletonBoneCount(); 
        globalTransforms = std::vector<BoneTransform>(boneCount); 
        animSeque1 = std::vector<BoneTransform>(boneCount);

        for (int i = 0; i < boneCount; i++)
        {
            GetSkeletonBoneLocalBindTransform(i,
                globalTransforms[i].pos.x, globalTransforms[i].pos.y, globalTransforms[i].pos.z,
                globalTransforms[i].rot.w, globalTransforms[i].rot.x, globalTransforms[i].rot.y, globalTransforms[i].rot.z
            );

            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) 
            {
                CombineTransform(globalTransforms[parentIndex], globalTransforms[i]);
            }
        }

        size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");

        for (int i = 0; i < boneCount; ++i)
        {
            int parentIndex = GetSkeletonBoneParentIndex(i);
            float px, py, pz, qw, qx, qy, qz;
            GetAnimLocalBoneTransform("ThirdPersonWalk.anim", i, 1, px, py, pz, qw, qx, qy, qz);

            BoneTransform transform;
            transform.name = GetSkeletonBoneName(i);
            transform.pos = Vec3(px, py, pz);
            transform.rot = Quaternion(qw, qx, qy, qz);

            animSeque1.push_back(transform);

            if (parentIndex != -1)
                CombineTransform(animSeque1[parentIndex], animSeque1[i]);
        }
    }

    virtual void Update(float frameTime) override
    {
        for (int i = 0; i < boneCount; i++)
        {
            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) 
            {
                //DrawLine(globalTransforms[parentIndex].pos.x, globalTransforms[parentIndex].pos.y - 50, globalTransforms[parentIndex].pos.z, globalTransforms[i].pos.x, globalTransforms[i].pos.y - 50, globalTransforms[i].pos.z, 1, 0, 0);
            }
        }
        for (int i = 0; i < animSeque1.size(); ++i)
        {
            int parentIndex = GetSkeletonBoneParentIndex(i);
            //std::cout << walkAnim.animSequence[0][i].pos.x << "  " << walkAnim.animSequence[0][i].pos.y << "   " << walkAnim.animSequence[0][i].pos.z << std::endl;
            if (parentIndex != -1)
            {
                DrawLine(animSeque1[parentIndex].pos.x, animSeque1[parentIndex].pos.y - 50, animSeque1[parentIndex].pos.z, animSeque1[i].pos.x, animSeque1[i].pos.y - 50, animSeque1[i].pos.z, 0, 0, 1);
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

