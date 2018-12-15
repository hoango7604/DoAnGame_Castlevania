#pragma once
#include "GameObject.h"



class Zombie : public CGameObject
{

public:
	virtual void SetState(int state,int i=0);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

