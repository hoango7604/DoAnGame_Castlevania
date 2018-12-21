#include "Axe.h"
#include "BossBat.h"
#include "Dracula.h"

void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

						CGame::GetInstance()->bossheath -= 1;
						bossbat->isHurt = true;
						bossbat->hurtTime = GetTickCount();

						if (CGame::GetInstance()->bossheath <= 0)
						{
							bossbat->isDie = true;
							Simon::score += 100;
							CGame::GetInstance()->start_fight_boss = false;
						}
					}
					else if (dynamic_cast<Dracula *>(e->obj))
					{
						Dracula *dracula = dynamic_cast<Dracula *>(e->obj);
						if (dracula->state == DRACULA_STATE_ATTACK && !dracula->isHit)
						{
							CGame::GetInstance()->bossheath -= 1;
							dracula->isHit = true;

							if (CGame::GetInstance()->bossheath <= 0)
							{
								dracula->isDie = true;
								Simon::score += 100;
							}
						}
					}
					else
					{
						Simon::score += 100;
						enemy->isDie = true;
					}
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
			}
		}
	}
}

void Axe::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + AXE_BBOX_WIDTH;
	b = y + AXE_BBOX_HEIGHT;
}