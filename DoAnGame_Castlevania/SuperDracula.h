#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "Simon.h"

class SuperDracula : public Enemy
{
	bool check_wait = false;
	bool check_start_fly = false;
	bool check_fly = false;
	bool check_onsky = false;
	bool check_hit = false;
	bool check_hit_time = false;
	int jump = 1;
public:
	DWORD timer;
	DWORD timer_hit;
	Simon *simon;
	bool isHitted = false;
	bool isBleeding;
	DWORD hitTime;
	SuperDracula(Simon *simon) {
		timer = GetTickCount();
		this->simon = simon;
	};
	virtual void SetState(int state, int i = 0);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

