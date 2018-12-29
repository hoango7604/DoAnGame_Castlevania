#include "define.h"
#include "Simon.h"
#include "Whip.h"
#include "Zombie.h"
#include "Panther.h"
#include "RedBat.h"
#include "BossBat.h"
#include "Candle.h"
#include "BigFire.h"
#include "Dracula.h"
#include "SuperDracula.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Enemy *>(coObjects->at(i)))
		{
			if (dynamic_cast<BossBat *>(coObjects->at(i)))
			{
				BossBat *bossbat = dynamic_cast<BossBat *>(coObjects->at(i));

				float zl, zr, zt, zb;
				bossbat->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					CGame::GetInstance()->bossheath -= 1;
					bossbat->isHurt = true;
					bossbat->hurtTime = GetTickCount();
					bossbat->StartUntouchable();

					if (CGame::GetInstance()->bossheath <= 0)
					{
						bossbat->isDie = true;
						Simon::score += 100;
						CGame::GetInstance()->start_fight_boss = false;
						CGame::GetInstance()->bossheath = 16;
					}
				}
			}
			else if (dynamic_cast<Dracula *>(coObjects->at(i)))
			{
				Dracula *dracula = dynamic_cast<Dracula *>(coObjects->at(i));

				float zl, zr, zt, zb;
				dracula->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt - 10)
				{
					if (dracula->state == DRACULA_STATE_ATTACK && !dracula->isHit)
					{
						CGame::GetInstance()->bossheath -= 16;
						dracula->isHit = true;

						if (CGame::GetInstance()->bossheath <= 0)
						{
							dracula->isDie = true;
							Simon::score += 100;
							//CGame::GetInstance()->start_fight_boss = false;
							CGame::GetInstance()->bossheath = 16;
						}
					}
				}
			}
			else if (dynamic_cast<SuperDracula *>(coObjects->at(i)))
			{
				SuperDracula *superdracula = dynamic_cast<SuperDracula *>(coObjects->at(i));

				float zl, zr, zt, zb;
				superdracula->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					CGame::GetInstance()->bossheath -= 1;
					superdracula->isHitted = true;

					if (CGame::GetInstance()->bossheath <= 0)
					{
						superdracula->isDie = true;
						Simon::score += 100;
						//CGame::GetInstance()->start_fight_boss = false;
						CGame::GetInstance()->bossheath = 16;
					}
				}
			}
			else if (dynamic_cast<SuperDracula *>(coObjects->at(i)))
			{
				SuperDracula *superDracula = dynamic_cast<SuperDracula *>(coObjects->at(i));
				float zl, zr, zt, zb;
				superDracula->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					if (!superDracula->isUntouchable)
					{
						CGame::GetInstance()->bossheath -= 1;
						superDracula->isHitted = true;
						superDracula->isBleeding = false;
						superDracula->hitTime = GetTickCount();
						superDracula->StartUntouchable();
					}

					if (CGame::GetInstance()->bossheath <= 0)
					{
						superDracula->isDie = true;
						Simon::score += 100;
					}
				}
			}
			else if (!dynamic_cast<Panther *>(coObjects->at(i)))
			{
				Enemy *enemy = dynamic_cast<Enemy *>(coObjects->at(i));

				float zl, zr, zt, zb;
				enemy->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					enemy->isDie = true;
					Simon::score += 100;
				}
			}
		}
		else if (dynamic_cast<BigFire *>(coObjects->at(i)))
		{
			BigFire *bigfire = dynamic_cast<BigFire *>(coObjects->at(i));
			float zl, zr, zt, zb;
			bigfire->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zl && wr > (zl + zr) / 2 && wt > zt && wb < zb)
			{
				bigfire->isHitted = true;
			}
		}
		else if (dynamic_cast<Candle *>(coObjects->at(i)))
		{
			Candle *candle = dynamic_cast<Candle *>(coObjects->at(i));
			float zl, zr, zt, zb;
			candle->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zl && wr > (zl + zr) / 2 && wt > zt - 4 * SIMON_ONSTAIR_ERR_RANGE && wb < zb)
			{
				candle->isHitted = true;
			}
		}
	}
}

void Whip::Render()
{
	RenderBoundingBox();
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case 0:
		right = x + WHIP_LV0_BBOX_WIDTH;
		bottom = y + WHIP_LV0_BBOX_HEIGHT;
		break;
	case 1:
		right = x + WHIP_LV1_BBOX_WIDTH;
		bottom = y + WHIP_LV1_BBOX_HEIGHT;
		break;
	default:
		right = x + WHIP_LV2_BBOX_WIDTH;
		bottom = y + WHIP_LV2_BBOX_HEIGHT;
		break;
	}
}

Whip::~Whip()
{

}
