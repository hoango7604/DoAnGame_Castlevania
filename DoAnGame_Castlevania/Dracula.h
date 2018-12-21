#pragma once
#include "Enemy.h"
#include "Ground.h"

class Dracula : public Enemy
{
public:

	float min_x, max_x;
	bool isActivate;
	bool isHit;
	bool isRest;
	DWORD restCast;
	DWORD lastRestTime;

	Dracula (float min_x, float max_x)
	{
		isActivate = false;
		this->min_x = min_x;
		this->max_x = max_x;
	}

	void Activate() { isActivate = true; }
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};







