#pragma once
#include "Enemy.h"
#include "Ground.h"

class MerMan : public Enemy
{

public:
	
	bool isOnGround;
	bool isAttack;
	bool didAttack;
	DWORD attackTime;
	DWORD lastAttackTime;
	
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

