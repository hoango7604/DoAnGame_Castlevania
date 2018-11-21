#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "define.h"

class Simon : public CGameObject
{
	int action;
	int level;
	int untouchable;
	DWORD untouchable_start;

public:
	/*
	 * Thu tu uu tien:
	 * 0. Chet
	 * 1. Tan cong
	 * 2. Len xuong cau thang
	 * 3. Ngoi
	 * 4. Nhay
	 * 5. Di chuyen (co the vua di chuyen vua nhay duoc, con lai thi di chuyen khong ket hop duoc)
	 */

	bool isDead;
	bool isAttack;
	bool isAboutToBeOnStair;
	bool isOnStair;
	bool isMovingOnStair;
	bool isSit;
	bool isExitSit;
	bool isJump;
	bool isMoving;

	DWORD attackTime;

public:
	Whip *whip;

	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
		whip = new Whip();
	}

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		whip->SetPosition(x, y);
	}

	void SetState(int state);

	void SetAction(int action);
	int GetAction() { return action; }

	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
