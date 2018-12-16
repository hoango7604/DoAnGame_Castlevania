#include "define.h"
#include "Simon.h"
#include "Whip.h"
#include "Zombie.h"
#include "RedBat.h"
#include "Candle.h"
#include "BigFire.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Enemy *>(coObjects->at(i)))
		{
			Enemy *enemy = NULL;

			if (dynamic_cast<Zombie *>(coObjects->at(i)))
				enemy = dynamic_cast<Zombie *>(coObjects->at(i));
			else if (dynamic_cast<MerMan *>(coObjects->at(i)))
				enemy = dynamic_cast<MerMan *>(coObjects->at(i));
			else if (dynamic_cast<RedBat *>(coObjects->at(i)))
				enemy = dynamic_cast<RedBat *>(coObjects->at(i));

			if (enemy != NULL)
			{
				float zl, zr, zt, zb;
				enemy->GetBoundingBox(zl, zt, zr, zb);
				if (wl < zl && wr > zr && wt > zt && wb < zb)
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
