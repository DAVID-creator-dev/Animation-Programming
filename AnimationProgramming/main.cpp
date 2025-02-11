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

    float timer = 0.f;
    float timerLimit = 3.f;
    bool flipflop;

    std::vector<Animation*> anims;
    Animation* walkAnimation;
    Animation* runAnimation;

    virtual void Init() override
    {
        boneCount = 64;

        walkAnimation = new Animation("ThirdPersonWalk.anim");
        runAnimation = new Animation("ThirdPersonRun.anim");

        walkAnimation->LoadAnimation(boneCount);
        runAnimation->LoadAnimation(boneCount);

        anims.push_back(walkAnimation);
        anims.push_back(runAnimation);
    }

    virtual void Update(float frameTime) override
    {
        timer += frameTime;

        if (timer > timerLimit)
        {
            if (flipflop)
                flipflop = false;
            else
                flipflop = true;

            timer = 0.f;
        }

        if (flipflop)
        {
            runAnimation->BlendAnimation(runAnimation, boneCount);
            //walkAnimation->PlayAnimation(frameTime, boneCount, 15.0f);
        }
        else
        {
            runAnimation->PlayAnimation(frameTime, boneCount, 15.0f);
        }
    }

};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}
