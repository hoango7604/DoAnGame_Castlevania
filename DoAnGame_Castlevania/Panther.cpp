#include "Panther.h"
#include "define.h"

void Panther::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PANTHER_STATE_JUMP:
		isJump = true;
		isUse = true;
		vy = -PANTHER_JUMPING_SPEED;
		break;
	case PANTHER_STATE_RUN:
		vx = -PANTHER_WALKING_SPEED;
		break;
	case PANTHER_STATE_WAIT:
		vx = 0;
		nx = -1;
		break;
	}
}

void Panther::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	Enemy::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	// Sẽ luôn luôn chạy thẳng
	x += dx;

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block
		y += min_ty * dy + ny * 0.4f;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (isJump && e->nx == 0 && e->ny < 0)
				{
					isJump = false;
					this->nx = -this->nx;
					this->vx = -this->vx;
					this->state = PANTHER_STATE_RUN;
				}

				if (ny != 0) vy = 0;
			}
		}
	}
}

void Panther::Render()
{

	int ani = -1;

	switch (state)
	{
	case PANTHER_STATE_WAIT:
		ani = PANTHER_ANI_WAIT;
		break;
	case PANTHER_STATE_RUN:
		if (vx < 0)
			ani = PANTHER_ANI_RUN_LEFT;
		else if (vx > 0)
			ani = PANTHER_ANI_RUN_RIGHT;
		break;
	case PANTHER_STATE_JUMP:
		ani = PANTHER_ANI_JUMP;
		break;
	}

	if (isFrozen)
		animations[ani]->ResetFrame();

	if (ani != -1)
		animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void Panther::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + PANTHER_BBOX_WIDTH;
	bottom = y + PANTHER_BBOX_HEIGHT;
}