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

        blend->AddAnimation(walkAnimation);
        blend->AddAnimation(runAnimation);

        blend->SetCurrentAnimation(walkAnimation);
    }

    virtual void Update(float frameTime) override
    {
        timer += frameTime;
        if (timer > timerLimit)
        {
            timer = 0.0f;

            if (running) {
                blend->SetNextAnimation(walkAnimation, 0.2f);
                running = false; 
            }
            else {
                blend->SetNextAnimation(runAnimation, 0.2f);
                running = true; 
            }
        }
        
        if (running) {
            blend->PlayAnimation(frameTime, 30.f);
        }
        else {
            blend->PlayAnimation(frameTime, 15.f);
        }
    }
};

int main()
{
    
    CSimulation simulation;
    Run(&simulation, 1400, 800);
    
    return 0;
}
