#pragma once
#include "GameObject.h"
#include "Ground.h"
#define WHIPITEM_BBOX_WIDTH  32
#define WHIPITEM_BBOX_HEIGHT 32

class WhipItem : public CGameObject
{
	bool isEaten;
public:
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

