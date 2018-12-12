#include "HolyWater.h"

void HolyWater::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	float ml, mr, mt, mb;

	this->GetBoundingBox(ml, mt, mr, mb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<Candle *>(coObjects->at(i)) &&
			!dynamic_cast<BigFire *>(coObjects->at(i)) &&
			!dynamic_cast<Stair *>(coObjects->at(i)) &&
			!dynamic_cast<CheckStair *>(coObjects->at(i)) &&
			!dynamic_cast<Weapon *>(coObjects->at(i)) &&
			!dynamic_cast<Item *>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			float sl, sr, st, sb;

			coObjects->at(i)->GetBoundingBox(sl, st, sr, sb);

			// Xét các vật thể đi vào vùng lửa thánh
			if (ml < sr && mr > sl)
			{
				if (dynamic_cast<Zombie *>(e->obj) && isBurn)
				{
					Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
					zombie->SetState(ZOMBIE_STATE_DIE);
					isBurn = true;
				}
				else if (dynamic_cast<Panther *>(e->obj) && isBurn)
				{
					Panther *panther = dynamic_cast<Panther *>(e->obj);
					panther->isDie = true;
					isBurn = true;
				}
			}

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

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

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			// Gravity
			vy += SIMON_GRAVITY * dt;

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

				if (dynamic_cast<Ground *>(e->obj))
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
