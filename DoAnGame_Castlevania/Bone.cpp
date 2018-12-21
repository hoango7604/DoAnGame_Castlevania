#include "Bone.h"

void Bone::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + 5;
	bottom = y + 5;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	vy += SIMON_GRAVITY * dt;
	x += dx;
	y += dy;
}

void Bone::Render()
{
	
	animations[0]->Render(x, y);

	//RenderBoundingBox();
}
