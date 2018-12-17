#pragma once
#include "Ground.h"

class Wall : public Ground
{
public:

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

