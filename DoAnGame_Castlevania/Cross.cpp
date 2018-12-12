#include "Cross.h"
#include "Effect.h"

void Cross::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<CheckStair *>(coObjects->at(i)) &&
			!dynamic_cast<CheckPoint *>(coObjects->at(i)) &&
			!dynamic_cast<Stair *>(coObjects->at(i)) &&
			!dynamic_cast<Effect *>(coObjects->at(i)) &&
			!dynamic_cast<Item *>(coObjects->at(i)) &&
			!dynamic_cast<Ground *>(coObjects->at(i)) &&
			!dynamic_cast<Weapon *>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Cross::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Outrange
		if (nx > 0)
		{
			if (x > max_x)
			{
				if (!isReturn)
				{
					x = max_x;
					vx = -vx;
					dx = 0;
					isReturn = true;
				}
				else
					isExposed = true;
			}
		}
		else if (nx < 0)
		{
			if (x < max_x)
			{
				if (!isReturn)
				{
					x = max_x;
					vx = -vx;
					dx = 0;
					isReturn = true;
				}
				else
					isExposed = true;
			}
		}

		vector<LPGAMEOBJECT> *simonObject = new vector<LPGAMEOBJECT>;
		simonObject->push_back(simon);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);
		CalcPotentialCollisions(simonObject, coEvents);

		delete simonObject;

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

			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Zombie *>(e->obj))
				{
					Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
					zombie->SetState(ZOMBIE_STATE_DIE);
				}
				else if (dynamic_cast<BigFire *>(e->obj))
				{
					BigFire *bigfire = dynamic_cast<BigFire *>(e->obj);
					bigfire->isHitted = true;
				}
				else if (dynamic_cast<Candle *>(e->obj))
				{
					Candle *candle = dynamic_cast<Candle *>(e->obj);
					candle->isHitted = true;
				}
				else if (dynamic_cast<Panther *>(e->obj))
				{
					Panther *panther = dynamic_cast<Panther *>(e->obj);
					panther->isDie = true;
				}
				else if (dynamic_cast<Simon *>(e->obj))
				{
					isExposed = true;
				}
			}
		}
	}
}

void Cross::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + CROSS_BBOX_WIDTH;
	b = y + CROSS_BBOX_HEIGHT;
}
