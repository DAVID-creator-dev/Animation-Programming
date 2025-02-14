#include "Binding.h"

void Binding::HandleInput(float frameTime, Blend* blend)
{
    AdjustSpeed(frameTime);
    ProcessPause(blend);
    ProcessRewind(blend);
    ProcessBlend();
}

void Binding::UpdateBlendFactor(float& blendFactor, float frameTime)
{
    if (blending)
    {
        if (running)
        {
            blendFactor += frameTime;
            if (blendFactor > 1.0f)
            {
                blending = false;
                blendFactor = 1.0f;
            }
        }
        else if (walking)
        {
            blendFactor -= frameTime;
            if (blendFactor < 0.0f)
            {
                blending = false;
                blendFactor = 0.0f;
            }
        }
    }
}

void Binding::ProcessPause(Blend* blend)
{
    if ((GetKeyState(VK_SPACE) & 0x8000) && canPause)
    {
        blend->ChangeAnimState(); 
        canPause = false;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x0001)
    {
        canPause = true;
    }
}

void Binding::ProcessRewind(Blend* blend)
{
    if ((GetKeyState(VK_RIGHT) & 0x8000) && canRewind)
    {
        blend->ChangeBlendDirection();
        canRewind = false;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
    {
        canRewind = true;
    }
}

void Binding::ProcessBlend()
{
    if ((GetKeyState(VK_LEFT) & 0x8000) && canBlend)
    {
        canBlend = false;
        blending = true;

        if (walking)
        {
            walking = false;
            running = true;
        }
        else
        {
            running = false;
            walking = true;
        }
    }
    if (GetKeyState(VK_LEFT) & 0x0001)
    {
        canBlend = true;
    }
}

void Binding::AdjustSpeed(float frameTime)
{
    if (GetKeyState(VK_DOWN) & 0x8000)
    {
        if (blendSpeed > 1.0f)
            blendSpeed -= speed * frameTime;
    }

    if (GetKeyState(VK_UP) & 0x8000)
    {
        blendSpeed += speed * frameTime;
    }
}