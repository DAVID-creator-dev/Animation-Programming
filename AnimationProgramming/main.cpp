#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Simulation.h"
#include "math.h"
#include "Animation.h"

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
    bool running = false;

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

        blendFactor += frameTime; 
        if (blendFactor >= 1.0f ) {
            blendFactor = 1.0f; 
        }

        blend->PlayAnimation(frameTime, 30.f, blendFactor, walkAnimation, runAnimation);
    }
};

int main()
{
    
    CSimulation simulation;
    Run(&simulation, 1400, 800);
    
    return 0;
}
