#pragma once
#include "Ground.h"

class Wall : public Ground
{
public:
	float width, height;
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetBB(float x, float y) { this->width = x; this->height = y; };
};

