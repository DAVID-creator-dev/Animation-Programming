#pragma once
#include <windows.h>
#include "Animation.h"

class Binding
{
public :
	Binding() {}
	~Binding() {}

	bool canPause = true;
	bool blending = false;
	bool walk = true;
	bool running = false;
	bool canBlending = true;
	float timer = 0.0f;
	float blendSpeed = 30.0f;
	float speed = 5;
	bool canRewind = true;

	void Rewind(Blend* blend)
	{
		if (GetKeyState(VK_RIGHT) & 0x8000 && canRewind)
		{
			blend->ChangeBlendDirection();
			canRewind = false;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
			canRewind = true;
	}

	void Pause(Blend* blend)
	{
		if (GetKeyState(VK_SPACE) & 0x8000 && canPause)
		{
			blend->ChangeAnimState();
			canPause = false;
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
			canPause = true;
	}

	void Blend()
	{
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

		if (GetKeyState(VK_LEFT) & 0x0001)
			canBlending = true;
	}

	void ChangeSpeed(float frameTime)
	{
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			if (blendSpeed > 1.0f)
				blendSpeed -= 1.0f * speed * frameTime;
		}

		if (GetKeyState(VK_UP) & 0x8000)
		{
			blendSpeed += 1.0f * speed * frameTime;
		}
	}
};