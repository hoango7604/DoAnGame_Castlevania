#include "Dracula.h"

void Dracula::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{

	}
}

void Dracula::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + DRACULA_BBOX_WIDTH;
	bottom = y + DRACULA_BBOX_HEIGHT;
}

void Dracula::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);



	x += dx;
	y += dy;
}

void Dracula::Render()
{
	int ani = -1;



	animations[ani]->Render(x, y);

	//RenderBoundingBox();
}


