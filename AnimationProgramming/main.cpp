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
    BoneTransform() : name(nullptr), pos(Vec3(0, 0, 0)), rot(Quaternion(0, 0, 0, 0)) {}
    
    const char* name; 
    Vec3 pos; 
    Quaternion rot; 
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

        boneCount = GetSkeletonBoneCount(); 
        globalTransforms = std::vector<BoneTransform>(boneCount); 
        
        for (int i = 0; i < boneCount; i++) {
            GetSkeletonBoneLocalBindTransform(i,
                globalTransforms[i].pos.x, globalTransforms[i].pos.y, globalTransforms[i].pos.z,
                globalTransforms[i].rot.w, globalTransforms[i].rot.x, globalTransforms[i].rot.y, globalTransforms[i].rot.z
            );

            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) {
                CombineTransform(globalTransforms[parentIndex], globalTransforms[i]);
            }
        }
    }

    virtual void Update(float frameTime) override {
        // X axis
        //DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);
        // Y axis
        //DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);
        // Z axis
        //DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

        for (int i = 1; i < boneCount; i++) {
            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) {
                DrawLine(
                    globalTransforms[parentIndex].pos.x, globalTransforms[parentIndex].pos.y, globalTransforms[parentIndex].pos.z,
                    globalTransforms[i].pos.x, globalTransforms[i].pos.y, globalTransforms[i].pos.z,
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

