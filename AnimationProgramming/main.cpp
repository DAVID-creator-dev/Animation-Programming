#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Simulation.h"
#include "math.h"
#include "Animation.h"
#include <windows.h>

class CSimulation : public ISimulation
{
    int boneCount;
    int keyFrame;

    Animation* walkAnimation;
    Animation* runAnimation;
    Blend* blend;

    float blendFactor = 0.0f; 

    float timer = 0.0f;
    float timerLimit = 2.4f;
    float blendSpeed = 30.0f;
    float targetSpeed = 0.0f;
    bool canPause = true;
    bool blending = false;
    bool walk = true;
    bool running = false;
    bool canBlending = true;

    float speed = 5; 

    virtual void Init() override
    {
        blend = new Blend();
        
        boneCount = 64;

        walkAnimation = new Animation("ThirdPersonWalk.anim");
        runAnimation = new Animation("ThirdPersonRun.anim");

        walkAnimation->LoadAnimation();
        runAnimation->LoadAnimation();
    }

    virtual void Update(float frameTime) override
    {  
        if (GetKeyState(VK_SPACE) & 0x8000 && canPause)
        {
            blend->ChangeAnimState();
            canPause = false;
        }

        if (GetAsyncKeyState(VK_SPACE) & 0x0001)
            canPause = true;

        if (GetKeyState(VK_DOWN) & 0x8000)
        {
            if (blendSpeed > 1.0f)
                blendSpeed -= 1.0f * speed * frameTime;

        }

        if (GetKeyState(VK_UP) & 0x8000)
        {
            blendSpeed += 1.0f * speed * frameTime;
        }

        if (GetKeyState(VK_LEFT) & 0x8000 && canBlending)
        {
            canBlending = false;
            blending = true;

            if (walk)
            {
                walk = false;
                running = true;
            }
            else
            {
                running = false;
                walk = true;
            }

            timer = 0.0f;
        }

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
            else if (walk)
            {
                blendFactor -= frameTime;
                if (blendFactor < 0.0f)
                {
                    blending = false;
                    blendFactor = 0.0f;
                }
            }

        }

        if (GetKeyState(VK_LEFT) & 0x0001)
            canBlending = true;

        blend->PlayAnimation(frameTime, blendSpeed, blendFactor, walkAnimation, runAnimation);
    }
};

int main()
{
    
    CSimulation simulation;
    Run(&simulation, 1400, 800);
    
    return 0;
}
