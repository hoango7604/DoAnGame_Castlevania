#pragma once
#include "Enemy.h"
#include "Game.h"
#include "Simon.h"

class BigBat : public Enemy
{
public:

	Simon *simon;

	bool isHurt;
	bool isBleeding;
	bool isWait;
	bool isActivate;

	DWORD waitTime;
	DWORD attackTime;
	DWORD hurtTime;

	float destination_x, destination_y;

	BigBat(Simon *simon) { this->simon = simon; maxHealth = 16; health = 16; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

