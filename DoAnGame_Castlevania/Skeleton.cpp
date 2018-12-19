#include "Skeleton.h"

void Skeleton::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	
	}
}

void Skeleton::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BBOX_WIDTH;
	bottom = y + SKELETON_BBOX_HEIGHT;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	

	x += dx;
	y += dy;
}

void Skeleton::Render()
{
	int ani = 0;

	

	animations[ani]->Render(x, y);

	//RenderBoundingBox();
}

