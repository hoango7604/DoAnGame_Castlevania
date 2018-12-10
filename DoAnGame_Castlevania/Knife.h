#pragma once
#include "Weapon.h"

class Knife : public Weapon
{

public:

	Knife(Simon *simon, float max_width = SCREEN_WIDTH) { this->simon = simon; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};