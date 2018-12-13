#include "MerMan.h"
#include "MerMan.h"
#include "define.h"

void MerMan::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MERMAN_STATE_JUMP:
		vy = -0.8;
		break;
	case MERMAN_STATE_DIE:
		vx = vy = 0;
		break;
	case MERMAN_STATE_WALK:
		vx = 0.5*SIMON_WALKING_SPEED;
		break;

	}
	if (x < 0 && vx <0)
	{
		x = 0;
		vx = -vx;
	}
	if (x > MAX_WIDTH_LV2_2 -2*MERMAN_BBOX_WIDTH && vx > 0)
	{
		vx = -vx;
		x = MAX_WIDTH_LV2_2 - 2 * MERMAN_BBOX_WIDTH;
	}
}

void MerMan::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		bool willBlock = false;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block
		

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{							
				if (e->ny < 0)
				{											
					willBlock = true;
					isonGround = true;
					// Xét va chạm cứng
					if (ny != 0) vy = 0;
				}
								
			}
		}
		if (willBlock)
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}
		else
		{
			x += dx;
			y += dy;
		}
		if (isonGround)
		{
			this->SetState(MERMAN_STATE_WALK);
		}
	}
}

void MerMan::Render()
{
	int ani = -1;

	switch (state)
	{
	case MERMAN_STATE_JUMP:
		ani = MERMAN_ANI_JUMP;
		break;
	case MERMAN_STATE_WALK:
		if (vx < 0)
			ani = MERMAN_ANI_WALK_LEFT;
		else if (vx > 0)
			ani = MERMAN_ANI_WALK_RIGHT;
		break;
	/*case MERMAN_STATE_DIE:
		ani = -1;
		break;*/
	}
	
	if (ani != -1)
		animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void MerMan::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + MERMAN_BBOX_WIDTH;
	bottom = y + MERMAN_BBOX_HEIGHT;
}
