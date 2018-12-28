#pragma once
#include "GameObject.h"

class Enemy : public CGameObject
{
public:
	int maxHealth;
	int health;
	int damage;

	bool isFrozen;
	bool isDie;

	bool isUntouchable;
	DWORD untouchable_start;
	DWORD untouchableTime;

	Enemy(){ maxHealth = 1; health = 1; damage = 1; }

	virtual void SetState(int state);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);

	void StartUntouchable(DWORD time = ENEMY_UNTOUCHABLE_TIME) { isUntouchable = true; untouchable_start = GetTickCount(); untouchableTime = time; }
};

