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
				
				if (e->nx != 0)
				{
					vx = -vx;					
				}

			}
		}
	}
	if (x > 1320)
	{
		vx = -vx;
		x = x - 20;
		old_cordinate = x;
	}
	else
		if (abs(old_cordinate - x) > 30 && abs(old_cordinate - x) < 50)
		{
			int i = rand() % (2);
			if (i == 0)
				vx = -vx;
			else
				vx = vx;
			old_cordinate = x;
		}
	
	
	
}

void Skeleton::Render()
{
	int ani = 0;
	
	animations[1]->Render(x, y);	
	
	//RenderBoundingBox();
}

