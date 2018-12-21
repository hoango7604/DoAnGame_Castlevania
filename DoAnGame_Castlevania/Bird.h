#pragma once
#include "Enemy.h"
#include "Ground.h"

class Bird : public Enemy
{
public:

	bool isDropHunchBack;
	bool isRest;
	DWORD restCat;
	DWORD lastRestTime;

	Bird (float nx) 
	{
		this->nx = nx;

		if (nx > 0)
		{
			vx = BIRD_FLYING_SPEED;
		}
		else
		{
			vx = -BIRD_FLYING_SPEED;
		}
	}

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};





