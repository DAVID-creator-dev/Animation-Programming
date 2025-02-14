#pragma once
#include <windows.h>
#include "Blend.h"

class Binding
{
private:
    bool canPause = true;
    bool canRewind = true;
    bool canBlend = true;

    bool blending = false;
    bool walking = true;
    bool running = false;

    float blendSpeed = 30.0f;
    float speed = 5.0f;

public:
    Binding() = default;
    ~Binding() = default;

    void HandleInput(float frameTime, Blend* blend);
    void UpdateBlendFactor(float& blendFactor, float frameTime);

    bool IsRunning() const { return running; }
    bool IsWalking() const { return walking; }
    bool IsBlending() const { return blending; }
    float GetBlendSpeed() const { return blendSpeed; }

private:
    void ProcessRewind(Blend* blend);
    void ProcessPause(Blend* blend);
    void ProcessBlend();
    void AdjustSpeed(float frameTime);
};
