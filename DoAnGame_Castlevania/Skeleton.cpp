#include "Skeleton.h"

void Skeleton::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case SKELETON_WALKING:
		vx = SKELETON_WALKING_SPEED;
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
	
	if (GetTickCount() - timer > 800)
	{
		vx = SKELETON_WALKING_SPEED;
		timer = GetTickCount();
	}
	else 
		vx = 0;
	
	
	
}

void Skeleton::Render()
{
	int ani = 0;
	
	animations[1]->Render(x, y);
	

	//RenderBoundingBox();
}

