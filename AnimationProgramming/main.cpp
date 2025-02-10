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

Quaternion globalQuaternion = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

struct BoneTransform {
    BoneTransform() : pos(Vec3(0.0f, 0.0f, 0.0f)), rot(Quaternion(0.0f, 0.0f, 0.0f, 1.0f)), mat(Mat4()){}

    Vec3 pos;
    Quaternion rot;
    Mat4 mat;
};

void CombineTransform(const BoneTransform& parent, BoneTransform& children) {
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
    std::vector<BoneTransform> bindPoseTransforms;
    std::vector<BoneTransform> globalTransforms;
    std::vector<BoneTransform> animeSeq; 
    std::vector<float> skinningData;

    virtual void Init() override {
        boneCount = 64; 
        globalTransforms = std::vector<BoneTransform>(boneCount);
        bindPoseTransforms = std::vector<BoneTransform>(boneCount); 
        animeSeq = std::vector<BoneTransform>(boneCount);

        skinningData = std::vector<float>(boneCount * 16);

        for (int i = 0; i < boneCount; i++) {
            GetSkeletonBoneLocalBindTransform(i,
                globalTransforms[i].pos.x, globalTransforms[i].pos.y, globalTransforms[i].pos.z,
                globalTransforms[i].rot.w, globalTransforms[i].rot.x, globalTransforms[i].rot.y, globalTransforms[i].rot.z
            );

            GetAnimLocalBoneTransform("ThirdPersonWalk.anim", i, 5, animeSeq[i].pos.x, animeSeq[i].pos.y, animeSeq[i].pos.z, animeSeq[i].rot.w, animeSeq[i].rot.x, animeSeq[i].rot.y, animeSeq[i].rot.z);

            bindPoseTransforms[i] = globalTransforms[i];

            int parentIndex = GetSkeletonBoneParentIndex(i);
            if (parentIndex != -1) {
                CombineTransform(globalTransforms[parentIndex], globalTransforms[i], animeSeq[i]);
                CombineTransform(bindPoseTransforms[parentIndex], bindPoseTransforms[i]);
            }
        }

        for (size_t i = 0; i < boneCount; i++) {

            std::cout << "Bone " << i << " Offset Matrix:\n";
            for (int j = 0; j < 16; j++) {
                std::cout << globalTransforms[i].mat.data[j] << " ";
                if (j % 4 == 3) std::cout << std::endl;
            }

            globalTransforms[i].mat = globalTransforms[i].mat * bindPoseTransforms[i].mat.InvertMatrix();
            globalTransforms[i].mat.TransposeMatrix();

            std::cout << "Bone " << i << " Offset Matrix:\n";
            for (int j = 0; j < 16; j++) {
                std::cout << globalTransforms[i].mat.data[j] << " ";
                if (j % 4 == 3) std::cout << std::endl;
            }
        }

        for (size_t i = 0; i < boneCount; i++) {
            std::memcpy(&skinningData[i * 16], globalTransforms[i].mat.data, 16 * sizeof(float));
        }
    }

    virtual void Update(float frameTime) override {
        if (skinningData.empty()) {
            std::cerr << "Error: skinningData is empty!" << std::endl;
            return;
        }
        else {
            SetSkinningPose(skinningData.data(), 64);
        }

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

