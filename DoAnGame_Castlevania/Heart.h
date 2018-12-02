#pragma once
#include "GameObject.h"
#include "Ground.h"
#define HEART_BBOX_WIDTH  16
#define HEART_BBOX_HEIGHT 16

class Heart : public CGameObject
{
	bool isEaten;
public:
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

