#pragma once
#include "Enemy.h"
#include "Game.h"
#include "Simon.h"

class BossBat : public Enemy
{

public:
	Simon *simon;
	CGame *game;

	bool isHurt;
	bool isWait;
	bool isAttack;
	bool isTop;
	bool isBottom;

	DWORD waitTime;
	DWORD attackTime;
	DWORD hurtTime;

	float destination_x, destination_y;

	BossBat(Simon *simon, CGame *game) { this->simon = simon; this->game = game; maxHealth = 16; health = 16; }

	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};
