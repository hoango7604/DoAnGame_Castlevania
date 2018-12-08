#pragma once
#include <algorithm>
#include "debug.h"

#include "GameObject.h"
#include "Whip.h"
#include "define.h"
#include "Game.h"
#include "BigFire.h"
#include "Candle.h"
#include "Item.h"
#include "Zombie.h"
#include "Ground.h"
#include "Stair.h"
#include "CheckStair.h"
#include "CheckPoint.h"
#include "Panther.h"

class Simon : public CGameObject
{
	int action;
	int level;
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
	bool isUseWhip;

	bool isOnCheckStairUp;
	bool isOnCheckStairDown;
	bool isOnStair;
	bool isLeftToRight;

	bool isSit;
	bool isExitSit;

	bool isJump;

	bool isMoving;

	bool isHurt;
	bool isUntouchable;

	DWORD attackTime;
	DWORD onCheckPointTime;
	int ny;

	int life;
	int preHP;
	bool isLevelUp;
	bool isBonus;

public:
	Whip * whip;
	static int score;
	static int heartsAmount;
	int currentWeapon;
	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		whip = new Whip();
		life = 3;
		preHP = 16;
		currentWeapon = 0;
	}

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetCurrentWeapon(int weapon) { currentWeapon = weapon; }
	int GetCurrentWeapon() { return currentWeapon; }

	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		whip->SetPosition(x, y);
	}
	void IncHeart(int num) { heartsAmount += num; }
	void IncScore(int num) { score += num; }
	void SetState(int state);

	void SetAction(int action);
	int GetAction() { return action; }

	void SetLevel(int l) { level = l; }
	void StartUntouchable() { isUntouchable = true; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	int GetScore() { return score; }
	int GetLife() { return life; }
	int GetHP() { return preHP; }
	int GetHeart() { return heartsAmount; }
};