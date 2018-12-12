#pragma once
#include "Zombie.h"
#include "BigFire.h"
#include "Candle.h"
#include "Panther.h"
#include "Stair.h"
#include "CheckStair.h"
#include "Simon.h"

class Weapon : public CGameObject
{

public:
	float max_width;
	float max_x;
	Simon *simon;

	bool isExposed;
	bool isActivate;
	DWORD firstCast;

	Weapon(){}

	Weapon(Simon *simon, float max_width = SCREEN_WIDTH) { this->simon = simon; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
};

