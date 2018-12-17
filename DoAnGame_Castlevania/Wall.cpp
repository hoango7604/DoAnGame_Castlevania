#include "Wall.h"

void Wall::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + WALL_BBOX_WIDTH;
	b = y + WALL_BBOX_HEIGHT;
}