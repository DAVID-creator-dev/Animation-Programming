#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Simulation.h"
#include "math.h"
#include "Binding.h"

class CSimulation : public ISimulation
{
    int boneCount;
    int keyFrame;

    Animation* walkAnimation;
    Animation* runAnimation;
    Blend* blend;
    Binding* binding;

    float blendFactor = 0.0f; 

    virtual void Init() override
    {
        blend = new Blend();
        binding = new Binding();

        boneCount = 64;

        walkAnimation = new Animation("ThirdPersonWalk.anim");
        runAnimation = new Animation("ThirdPersonRun.anim");

        walkAnimation->LoadAnimation();
        runAnimation->LoadAnimation();
    }

    virtual void Update(float frameTime) override
    {  
        binding->Pause(blend);
        binding->Rewind(blend);
        binding->ChangeSpeed(frameTime);
        binding->Blend();

        if (blend)
        {
            if (binding->running)
            {
                blendFactor += frameTime;
                if (blendFactor > 1.0f)
                {
                    binding->blending = false;
                    blendFactor = 1.0f;
                }
            }
            else if (binding->walk)
            {
                blendFactor -= frameTime;
                if (blendFactor < 0.0f)
                {
                    binding->blending = false;
                    blendFactor = 0.0f;
                }
            }
        }

        blend->PlayAnimation(frameTime, binding->blendSpeed, blendFactor, walkAnimation, runAnimation);
    }
};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);
    
    return 0;
}
