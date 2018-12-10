#pragma once
#include "Weapon.h"

class Axe : public Weapon
{
public:

	Axe(Simon *simon, float max_width = SCREEN_WIDTH) { this->simon = simon; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

