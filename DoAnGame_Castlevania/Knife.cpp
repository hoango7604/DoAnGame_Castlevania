#include "Knife.h"
#include "BossBat.h"

void Knife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		x += dx;
		y += dy;

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{

		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Enemy *>(e->obj))
				{
					Enemy *enemy = dynamic_cast<Enemy *>(e->obj);

					if (dynamic_cast<BossBat *>(e->obj))
					{
						BossBat *bossbat = dynamic_cast<BossBat *>(e->obj);

						bossbat->health -= 1;
						bossbat->isHurt = true;
						bossbat->hurtTime = GetTickCount();

						if (bossbat->health == 0)
						{
							bossbat->isDie = true;
							Simon::score += 100;
						}
					}
					else
						enemy->isDie = true;

					this->isExposed = true;
				}
				else if (dynamic_cast<BigFire *>(e->obj))
				{
					BigFire *bigfire = dynamic_cast<BigFire *>(e->obj);
					bigfire->isHitted = true;
					this->isExposed = true;
				}
				else if (dynamic_cast<Candle *>(e->obj))
				{
					Candle *candle = dynamic_cast<Candle *>(e->obj);
					candle->isHitted = true;
					this->isExposed = true;
				}
			}
		}
	}
}

void Knife::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + KNIFE_BBOX_WIDTH;
	b = y + KNIFE_BBOX_HEIGHT;
}
