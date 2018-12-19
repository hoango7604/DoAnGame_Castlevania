#pragma once
#include "Enemy.h"
#include "Ground.h"

class Skeleton : public Enemy
{
public:
		
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

