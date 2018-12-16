#pragma once
#include "GameObject.h"

class Enemy : public CGameObject
{
public:
	int maxHealth;
	int health;
	int damage;

	bool isDie;

	Enemy(){ maxHealth = 1; health = 1; damage = 1; }

	virtual void SetState(int state);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};
