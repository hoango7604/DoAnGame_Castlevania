#pragma once
#include "GameObject.h"
#include "Whip.h"
#include"define.h"

class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	
	
public:
	Whip *whip;
	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
		whip = new Whip();
	}
	 
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		whip->SetPosition(x, y);
	}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
