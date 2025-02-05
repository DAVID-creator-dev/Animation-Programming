#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Engine.h"
#include "Simulation.h"
#include "math.h"

class CSimulation : public ISimulation
{
	std::vector<Vec3> positionsBones;

	virtual void Init() override
	{
		size_t size = GetSkeletonBoneCount();
		size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");

		for (int i = 0; i < GetSkeletonBoneCount(); ++i)
		{
			float posX, posY, posZ, quatW, quatX, quatY, quatZ;
			GetSkeletonBoneLocalBindTransform(i, posX, posY, posZ, quatW, quatX, quatY, quatZ);
			//Vec3 v = Matrix4x4::Identity4x4() * InitVector(posX, posY, posZ);
			positionsBones.push_back(Vec3(posX, posY, posZ));

			if (i > 0)
			{
				int parent = GetSkeletonBoneParentIndex(i);
				printf(GetSkeletonBoneName(parent));
				printf("\n");
			}
		}
	}

	virtual void Update(float frameTime) override
	{
		for (int i = 0; i < positionsBones.size(); ++i)
		{
			int p = GetSkeletonBoneParentIndex(i);
			if (i != positionsBones.size() - 1 && p != -1)
			{
				DrawLine(positionsBones[i].x, positionsBones[i].y, positionsBones[i].z, positionsBones[p].x, positionsBones[p].y, positionsBones[p].z, 0, 0, 1);
			}
			//DrawLine(positionsBones[1].x, positionsBones[1].y, positionsBones[1].z, positionsBones[2].x, positionsBones[2].y, positionsBones[2].z, 0, 0, 1);
			//DrawLine(positionsBones[2].x, positionsBones[2].y, positionsBones[2].z, positionsBones[3].x, positionsBones[3].y, positionsBones[3].z, 0, 0, 1);
			//DrawLine(positionsBones[3].x, positionsBones[3].y, positionsBones[3].z, positionsBones[4].x, positionsBones[4].y, positionsBones[4].z, 0, 0, 1);
			//DrawLine(positionsBones[4].x, positionsBones[4].y, positionsBones[4].z, positionsBones[5].x, positionsBones[5].y, positionsBones[5].z, 0, 0, 1);
			//DrawLine(positionsBones[5].x, positionsBones[5].y, positionsBones[5].z, positionsBones[6].x, positionsBones[6].y, positionsBones[6].z, 0, 0, 1);
			//DrawLine(positionsBones[6].x, positionsBones[6].y, positionsBones[6].z, positionsBones[7].x, positionsBones[7].y, positionsBones[7].z, 0, 0, 1);
			//DrawLine(positionsBones[7].x, positionsBones[7].y, positionsBones[7].z, positionsBones[8].x, positionsBones[8].y, positionsBones[8].z, 0, 0, 1);
			//DrawLine(positionsBones[8].x, positionsBones[8].y, positionsBones[8].z, positionsBones[9].x, positionsBones[9].y, positionsBones[9].z, 0, 0, 1);
			//DrawLine(positionsBones[9].x, positionsBones[9].y, positionsBones[9].z, positionsBones[10].x, positionsBones[10].y, positionsBones[10].z, 0, 0, 1);
		}
	}
};

int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}

