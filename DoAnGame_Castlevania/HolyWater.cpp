#include "HolyWater.h"
#include "BossBat.h"
#include "Dracula.h"

void HolyWater::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	float ml, mr, mt, mb;

	this->GetBoundingBox(ml, mt, mr, mb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<Stair *>(coObjects->at(i)) &&
			!dynamic_cast<CheckStair *>(coObjects->at(i)) &&
			!dynamic_cast<Weapon *>(coObjects->at(i)) &&
			!dynamic_cast<Item *>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			float sl, sr, st, sb;

			coObjects->at(i)->GetBoundingBox(sl, st, sr, sb);

			// Xét các vật thể đi vào vùng lửa thánh
			if (ml < sr && mr > sl && mt < sb && mb > st)
			{
				if (dynamic_cast<Enemy *>(e->obj) && isBurn)
				{
					Enemy *enemy = dynamic_cast<Enemy *>(e->obj);

					if (dynamic_cast<BossBat *>(e->obj))
					{
						BossBat *bossbat = dynamic_cast<BossBat *>(e->obj);

						CGame::GetInstance()->bossheath -= 1;
						bossbat->isHurt = true;
						bossbat->hurtTime = GetTickCount();

						if (CGame::GetInstance()->bossheath == 0)
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

							if (CGame::GetInstance()->bossheath == 0)
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

					isBurn = true;
				}
				else if ((dynamic_cast<BigFire *>(e->obj) && isBurn))
				{
					BigFire *bigFire = dynamic_cast<BigFire *>(e->obj);
					bigFire->isHitted = true;
					isBurn = true;
				}
				else if ((dynamic_cast<Candle *>(e->obj) && isBurn))
				{
					Candle *candle = dynamic_cast<Candle *>(e->obj);
					candle->isHitted = true;
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
