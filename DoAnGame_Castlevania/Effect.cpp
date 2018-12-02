#include "Effect.h"


void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void Effect::Render()
{
	//animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Effect::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x ;
	b = y ;
}

