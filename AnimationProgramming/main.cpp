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
        runAnimation->PlayAnimation(frameTime, boneCount, 15.0f);


    }

};

int main()
{
    CSimulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}
