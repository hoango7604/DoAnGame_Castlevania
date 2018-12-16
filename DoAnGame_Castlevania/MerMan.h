#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "WaterAnimation.h"

class MerMan : public Enemy
{

public:
	
	bool isOnGround;
	bool isAttack;
	bool didAttack;
	DWORD attackTime;
	DWORD lastAttackTime;
	
	WaterAnimation * wa_ani[3];
	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		for (int i = 0; i < 3; i++)
		{
			wa_ani[i] = new WaterAnimation();
			wa_ani[i]->SetSpeed(0, -0.0055);
		}
		wa_ani[0]->SetPosition(x - 32, y);
		wa_ani[1]->SetPosition(x +32, y);
		wa_ani[2]->SetPosition(x , y-32);
		
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

