#include "vector"
#include "math.h"
#include "map"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Simulation.h"
#include "math.h"

#include "Blend.h"
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
        binding->HandleInput(frameTime, blend);
        binding->UpdateBlendFactor(blendFactor, frameTime);

        blend->PlayAnimation(frameTime, binding->GetBlendSpeed(), blendFactor, walkAnimation, runAnimation); 
    }
};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);
    
    return 0;
}
