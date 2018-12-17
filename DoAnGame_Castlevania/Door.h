#pragma once
#include "GameObject.h"
#include"Game.h"
#include"define.h"


class Door : public CGameObject
{
	int check =0;
	bool check1 = false;
	bool check2 = false;
	DWORD time;
	DWORD time1;
	DWORD time2;
public:
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

