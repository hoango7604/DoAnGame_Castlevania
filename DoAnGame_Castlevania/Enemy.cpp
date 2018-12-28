#include "Enemy.h"
#include "Candle.h"
#include "BigFire.h"
#include "Stair.h"
#include "CheckStair.h"
#include "Weapon.h"
#include "Item.h"
#include "Bone.h"

void Enemy::SetState(int state)
{
	CGameObject::SetState(state);
}

void Enemy::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Simon se khong va cham voi nhung vat sau:
		if (!dynamic_cast<Candle *>(coObjects->at(i)) &&
			!dynamic_cast<BigFire *>(coObjects->at(i)) &&
			!dynamic_cast<Stair *>(coObjects->at(i)) &&
			!dynamic_cast<CheckStair *>(coObjects->at(i)) &&			
			!dynamic_cast<Weapon *>(coObjects->at(i)) &&
			!dynamic_cast<Enemy *>(coObjects->at(i))
			)
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// reset untouchable timer if untouchable time has passed
	if (isUntouchable && GetTickCount() - untouchable_start > untouchableTime)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}

	CGameObject::Update(dt, coObjects);
}