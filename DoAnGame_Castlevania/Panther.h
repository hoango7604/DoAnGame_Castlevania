#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Ground.h"

class Panther : public CGameObject
{

public:
	bool isActivate;
	bool isUse;
	bool isJump;
	bool isDie;

	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

