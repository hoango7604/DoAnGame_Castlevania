#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "Bone.h"
class Skeleton : public Enemy
{
	
	
	float old_cordinate;
	float position;
public:
	Skeleton () {
		timer = GetTickCount();
	}
	bool isAttacking =false;
	DWORD timer;
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void SetPosition(float x, float y) { this->x = x; this->y = y;  old_cordinate =position= x; }
};

