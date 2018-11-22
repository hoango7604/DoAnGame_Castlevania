#include "define.h"

#include "Whip.h"
#include "Zombie.h"
#include "Candle.h"
#include "BigFire.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Zombie *>(coObjects->at(i)))
		{
			Zombie *zombie = dynamic_cast<Zombie *>(coObjects->at(i));
			float zl, zr, zt, zb;
			zombie->GetBoundingBox(zl, zt, zr, zb);
			if (x < zl && x + WHIP_BBOX_WIDTH > zr && y > zt && y + WHIP_BBOX_HEIGHT < zb)
			{
				if (zombie->GetState() != ZOMBIE_STATE_DIE) {
					zombie->SetState(ZOMBIE_STATE_DIE);
				}
			}
		}
		else if (dynamic_cast<BigFire *>(coObjects->at(i)))
		{
			BigFire *bigfire = dynamic_cast<BigFire *>(coObjects->at(i));
			float zl, zr, zt, zb;
			bigfire->GetBoundingBox(zl, zt, zr, zb);
			if (x < zl && x + WHIP_BBOX_WIDTH > zr && y > zt && y + WHIP_BBOX_HEIGHT < zb)
			{
				if (bigfire->GetState() != ZOMBIE_STATE_DIE) {
					bigfire->SetState(ZOMBIE_STATE_DIE);
				}
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
	right = x + WHIP_BBOX_WIDTH;
	bottom = y + WHIP_BBOX_HEIGHT;
}

Whip::~Whip()
{

}
