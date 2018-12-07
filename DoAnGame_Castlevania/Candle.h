#pragma once
#include "GameObject.h"
#include "define.h"

class Candle : public CGameObject
{
public:
	bool isHitted;
	Candle() :CGameObject() {
		isHitted = false;
	}
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

