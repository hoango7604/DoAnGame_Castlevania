#include "HolyWater.h"

void HolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Outrange
		if (nx > 0)
		{
			if (this->x > this->max_x)
				isExposed = true;
		}
		else if (nx < 0)
		{
			if (this->x < this->max_x)
				isExposed = true;
		}

		// Outtime
		if (GetTickCount() - firstCast > ITEM_LIVE_TIME / 2)
		{
			isExposed = true;
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		// Gravity
		vy += SIMON_GRAVITY * dt;

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			bool willBlock = false;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Zombie *>(e->obj))
				{
					Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
					zombie->SetState(ZOMBIE_STATE_DIE);
					isBurn = true;
				}
				else if (dynamic_cast<Panther *>(e->obj))
				{
					Panther *panther = dynamic_cast<Panther *>(e->obj);
					panther->isDie = true;
					isBurn = true;
				}
				else if (dynamic_cast<Ground *>(e->obj))
				{
					if (e->ny < 0)
					{
						vx = 0;
						vy = 0;
						isBurn = true;
						willBlock = true;
					}
				}
			}

			// block 
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
		}
	}
}

void HolyWater::Render()
{
	if (isBurn)
		animations[1]->Render(x, y);
	else if (isActivate)
		animations[0]->Render(x, y);
}

void HolyWater::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + HOLYWATER_BBOX_WIDTH;
	b = y + HOLYWATER_BBOX_HEIGHT;
}
