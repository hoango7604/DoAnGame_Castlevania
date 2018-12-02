#include "BigFire.h"

void BigFire::Render()
{
	if (isHitted == false)	 
		animations[0]->Render(x, y, 255);
	else
	{
		float hx, hy;
		heart->GetPosition(hx, hy);
		heart->animations[0]->Render(hx, hy, 255);
	}
	//RenderBoundingBox();
}
void BigFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	heart->Update(dt, coObjects);
}

void BigFire::SetHit()
{
	isHitted = true;
}

void BigFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIGFIRE_BBOX_WIDTH;
	b = y + BIGFIRE_BBOX_HEIGHT;
}
