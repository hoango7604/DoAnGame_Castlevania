#pragma once
#include "Weapon.h"

class Cross : public Weapon
{
public:
	bool isReturn;

	Cross(Simon *simon, float max_width = SCREEN_WIDTH) { this->simon = simon; this->max_width = max_width; }

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};