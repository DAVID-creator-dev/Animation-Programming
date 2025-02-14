#include "Blend.h"

Blend::Blend() {
    skinningData = std::vector<float>(BONECOUNT * 16);
}

void Blend::ChangeBlendDirection()
{
    isRewind = !isRewind;
}

void Blend::ChangeAnimState()
{
    isPaused = !isPaused;
}

void Blend::PlayAnimation(float frameTime, float blendSpeed, float blendFactor, Animation* anim1, Animation* anim2)
{
    if (!isPaused)
    {
        keyFrame = anim1->keyFrame;
        adjustedFrameTime = frameTime * blendSpeed;
        animationDuration = keyFrame - 1;

        currentTime += isRewind ? -adjustedFrameTime : adjustedFrameTime;

        if (currentTime < 0.0f)
            currentTime += animationDuration;
        else if (currentTime >= animationDuration)
            currentTime = 0.0f;

        if (isRewind)
        {
            currentKeyFrame = static_cast<int>(currentTime);
            nextKeyFrame = (currentKeyFrame - 1 + keyFrame) % keyFrame;
            t = 1.0f - (currentTime - currentKeyFrame);
        }
        else
        {
            currentKeyFrame = static_cast<int>(currentTime);
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

        if (anim1->parent[j] != -1)
        {
            int parentIndex = anim1->parent[j]; 

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

