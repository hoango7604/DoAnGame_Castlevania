#pragma once

#include "GameObject.h"

#define CHECKSTAIR_BBOX_WIDTH  32
#define CHECKSTAIR_BBOX_HEIGHT 32

class CheckStair : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
