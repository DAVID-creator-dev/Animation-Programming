#pragma once
#include "Animation.h"

class Blend
{
public:
    Blend();

    void PlayAnimation(float frameTime, float blendSpeed, float blendFactor, Animation* anim1, Animation* anim2);
    void ChangeBlendDirection();
    void ChangeAnimState();

private:
    std::vector<float> skinningData;
    float t = 0.0f;

    float currentTime = 0.0f;
    float adjustedFrameTime = 0.0f;
    float animationDuration = 0.0f;

    int keyFrame = 0;
    int currentKeyFrame = 0;
    int nextKeyFrame = 0;

    bool isPaused = false;
    bool isRewind = false;
};