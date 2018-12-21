#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "Simon.h"
#include "Game.h"

class Dracula : public Enemy
{
public:

	Simon *simon;
	CGame *game;

	float min_x, max_x;
	bool isActivate;
	bool isHit;
	bool isBleeding;
	bool didAttack;
	DWORD restCast;
	DWORD lastRestTime;

	Dracula (Simon *simon, CGame *game)
	{
		isActivate = false;
		this->simon = simon;
		this->game = game;

		maxHealth = 16; 
		health = 16;

		vy = -DRACULA_SPEED;
		nx = -1;
	}

	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};







