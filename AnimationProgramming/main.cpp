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


struct BoneTransform {
    const char* name; 
    float posX, posY, posZ;
    float quatW, quatX, quatY, quatZ;
};

void CombineTransform(float parent_x, float parent_y, float parent_z, float& local_x, float& local_y, float& local_z) {
    local_x += parent_x;
    local_y += parent_y;
    local_z += parent_z;
}

class CSimulation : public ISimulation
{

    virtual void Init() override {
        int spine01 = GetSkeletonBoneIndex("spine_01");
        int spineParent = GetSkeletonBoneParentIndex(spine01);
        const char* spineParentName = GetSkeletonBoneName(spineParent);

        float posX, posY, posZ, quatW, quatX, quatY, quatZ;
        size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
        GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, posX, posY, posZ, quatW, quatX, quatY, quatZ);

        printf("Spine parent bone : %s\n", spineParentName);
        printf("Anim key count : %ld\n", keyCount);
        printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);
    }

    virtual void Update(float frameTime) override {
        // X axis
        DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);
        // Y axis
        DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);
        // Z axis
        DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

        int boneCount = GetSkeletonBoneCount();
        std::vector<BoneTransform> globalTransforms(boneCount);

        for (int i = 0; i < boneCount; i++) {
            GetSkeletonBoneLocalBindTransform(i,
                globalTransforms[i].posX, globalTransforms[i].posY, globalTransforms[i].posZ,
                globalTransforms[i].quatW, globalTransforms[i].quatX, globalTransforms[i].quatY, globalTransforms[i].quatZ
            );

            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) {
                CombineTransform(
                    globalTransforms[parentIndex].posX, globalTransforms[parentIndex].posY, globalTransforms[parentIndex].posZ,
                    globalTransforms[i].posX, globalTransforms[i].posY, globalTransforms[i].posZ
                );
            }
        }

        for (int i = 1; i < boneCount; i++) {
            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) {
                DrawLine(
                    globalTransforms[parentIndex].posX, globalTransforms[parentIndex].posY, globalTransforms[parentIndex].posZ,
                    globalTransforms[i].posX, globalTransforms[i].posY, globalTransforms[i].posZ,
                    1, 0, 0 
                );
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

