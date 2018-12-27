#pragma once
#include "Enemy.h"
#include "Bird.h"
#include "Ground.h"
#include "Simon.h"

class Hunchback : public Enemy
{
public:
	Simon *simon;
	Bird *bird;
	bool isStickToBird;
	bool isStickToGround;
	bool isIdle;
	DWORD idleCast;

	Hunchback(Simon *simon, Bird *bird) 
	{
		this->simon = simon;
		this->bird = bird;
		isStickToBird = true;
		
		state = HUNCHBACK_STATE_JUMP;
		nx = bird->nx;
		x = bird->x;
		y = bird->y;
	}

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};



