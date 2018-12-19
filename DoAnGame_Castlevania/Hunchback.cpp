#include "Hunchback.h"

void Hunchback::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{

	}
}

void Hunchback::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + HUNCHBACK_BBOX_WIDTH;
	bottom = y + HUNCHBACK_BBOX_HEIGHT;
}

void Hunchback::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);



	x += dx;
	y += dy;
}

void Hunchback::Render()
{
	int ani = -1;



	animations[ani]->Render(x, y);

	//RenderBoundingBox();
}

