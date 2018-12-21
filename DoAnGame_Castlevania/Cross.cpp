#include "Cross.h"
#include "Effect.h"
#include "BossBat.h"
#include "Dracula.h"

void Cross::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	float ml, mr, mt, mb;

	this->GetBoundingBox(ml, mt, mr, mb);

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

			float sl, sr, st, sb;

			coObjects->at(i)->GetBoundingBox(sl, st, sr, sb);

			if (ml < sr && mr > sl && mt < sb && mb > st)
			{
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
					else if (dynamic_cast<SuperDracula *>(e->obj))
					{
						SuperDracula *superDracula = dynamic_cast<SuperDracula *>(e->obj);
						if (!superDracula->isHitted)
						{
							CGame::GetInstance()->bossheath -= 1;
							superDracula->isHitted = true;
							superDracula->hitTime = GetTickCount();
						}

						if (CGame::GetInstance()->bossheath <= 0)
						{
							superDracula->isDie = true;
							Simon::score += 100;
						}
					}
					else
					{
						Simon::score += 100;
						enemy->isDie = true;
					}
				}
				else if (dynamic_cast<Simon *>(e->obj) && isReturn)
				{
					isExposed = true;
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
		x += dx;
		y += dy;

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BigFire *>(e->obj))
			{
				BigFire *bigfire = dynamic_cast<BigFire *>(e->obj);
				bigfire->isHitted = true;
			}
			else if (dynamic_cast<Candle *>(e->obj))
			{
				Candle *candle = dynamic_cast<Candle *>(e->obj);
				candle->isHitted = true;
			}
			else if (dynamic_cast<Simon *>(e->obj))
			{
				isExposed = true;
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
