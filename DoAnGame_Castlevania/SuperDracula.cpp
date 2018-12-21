﻿#include "SuperDracula.h"
#include "define.h"

void SuperDracula::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SUPERDRACULA_BBOX_WIDTH;
	bottom = y + SUPERDRACULA_BBOX_HEIGHT;
}

void SuperDracula::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	
	if (isHitted == false)
	{
		if (GetTickCount() - timer > 2000 && check_wait == false)
		{
			state = SUPERDRACULA_START_FLY;
			check_wait = true;
		}
		if (GetTickCount() - timer > 4000 && check_start_fly == false)
		{
			state = SUPERDRACULA_FLY;
			check_start_fly = true;
		}
		if (GetTickCount() - timer > 6000 && check_onsky == false)
		{
			state = SUPERDRACULA_ON_SKY;
			check_onsky = true;
		}
		if (GetTickCount() - timer > 6500)
		{
			state = SUPERDRACULA_START_FLY;
		}
		if (GetTickCount() - timer > 7000)
		{
			state = SUPERDRACULA_WAIT;
			timer = GetTickCount();
			check_onsky = false;
			check_start_fly = false;
			check_wait = false;
		}
	}
	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{

				if (ny != 0) vy = 0;			

			}
		}
	}
	
}

void SuperDracula::Render()
{
	switch (state)
	{
	case SUPERDRACULA_WAIT:		
		animations[0]->Render(x, y);
		vx = 0;
		break;
	case SUPERDRACULA_START_FLY:
		animations[1]->Render(x, y);
		break;
	case SUPERDRACULA_FLY:
		animations[2]->Render(x, y);
		vy = -0.15;
		vx = -0.04;
		break;
	case SUPERDRACULA_ON_SKY:
		animations[3]->Render(x, y);
		vy = 0;
		break;
	}

}

void SuperDracula::SetState(int state, int i)
{
	Enemy::SetState(state);

	switch (state)
	{
	
	}
}

