#include "BigFire.h"

void BigFire::Render()
{
	if (isHited == false)
		animations[0]->Render(x, y,255);
	else
		animations[1]->Render(x, y,255);
	//RenderBoundingBox();
}
void BigFire::Update()
{

}

void BigFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIGFIRE_BBOX_WIDTH;
	b = y + BIGFIRE_BBOX_HEIGHT;
}
