#pragma once
#include "GameObject.h"
#include "Game.h"
class WaterAnimation : public CGameObject
{

public:	
	WaterAnimation() :CGameObject()
	{

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	~WaterAnimation();
};

