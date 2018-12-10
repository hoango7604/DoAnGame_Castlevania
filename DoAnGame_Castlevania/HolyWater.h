#pragma once
#include "Weapon.h"

class HolyWater : public Weapon
{

public:
	bool isBurn;

	HolyWater(Simon *simon, float max_width = SCREEN_WIDTH) { this->simon = simon; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

