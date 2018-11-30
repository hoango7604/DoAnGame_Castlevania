#include "BossBat.h"
#include "define.h"

void BossBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOSSBAT_BBOX_WIDTH;	
	bottom = y + BOSSBAT_BBOX_HEIGHT;
}

void BossBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;	

}

void BossBat::Render()
{
	int ani;
	

	if (state == BOSSBAT_STATE_WAIT)
		ani = BOSSBAT_ANI_WAIT;
	else if (state == BOSSBAT_STATE_ATTACK)
		ani = BOSSBAT_ANI_ATTACK;
	animations[ani]->Render(x, y);
	
	RenderBoundingBox();
}

void BossBat::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case BOSSBAT_STATE_WAIT:
			vx = vy = 0;
			break;
		

	}

}