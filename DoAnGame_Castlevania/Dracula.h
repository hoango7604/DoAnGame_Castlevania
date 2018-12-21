#pragma once
#include "Enemy.h"
#include "Ground.h"

class DraculaHead : public CGameObject
{
public:
	bool isHit;

	DraculaHead(){}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

class Dracula : public Enemy
{
public:
	DraculaHead *head;
	float min_x, max_x;
	bool isActivate;
	bool isHit;
	bool isRest;
	DWORD restCat;
	DWORD lastRestTime;

	Dracula (float min_x, float max_x)
	{
		head = new DraculaHead();
		isActivate = false;
		this->min_x = min_x;
		this->max_x = max_x;
	}

	void Activate() { isActivate = true; }
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};







