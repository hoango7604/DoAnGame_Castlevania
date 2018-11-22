#include "Panther.h"
#include "define.h"

void Panther::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + PANTHER_BBOX_WIDTH;

	if (state == PANTHER_STATE_DIE)
		bottom = y + PANTHER_BBOX_HEIGHT_DIE;
	else
		bottom = y + PANTHER_BBOX_HEIGHT;
}

void Panther::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);


	if (state == PANTHER_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x >3156) {
		x = 3156; vx = -vx;
	}


}

void Panther::Render()
{
	int ani;
	if (state == PANTHER_STATE_DIE) {
		ani = PANTHER_ANI_DIE;
		return;
	}
	if (state == PANTHER_STATE_WAIT) {
		ani = PANTHER_ANI_WAIT;
		
	}
	else {
		if (vx < 0)
			ani = PANTHER_ANI_RUN_LEFT;
		else if (vx > 0)
			ani = PANTHER_ANI_RUN_RIGHT;
		
	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Panther::SetState(int state)
{
	
	CGameObject::SetState(state);
	switch (state)
	{
	case PANTHER_STATE_WAIT:
		vx = 0;
		vy = 0;
		break;
	case PANTHER_STATE_ATTACK:
		vx = -PANTHER_WALKING_SPEED;
		vy = 0.6;
	}
}
