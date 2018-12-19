#include "Bird.h"

void Bird::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{

	}
}

void Bird::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BIRD_BBOX_WIDTH;
	bottom = y + BIRD_BBOX_HEIGHT;
}

void Bird::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);



	x += dx;
	y += dy;
}

void Bird::Render()
{
	int ani = -1;



	animations[0]->Render(x, y);

	//RenderBoundingBox();
}

