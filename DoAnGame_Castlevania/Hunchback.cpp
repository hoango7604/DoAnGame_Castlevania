#include "Hunchback.h"

void Hunchback::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Simon se khong va cham voi nhung vat sau:
		if (dynamic_cast<Ground *>(coObjects->at(i)))
		{
			Ground *ground = dynamic_cast<Ground *>(coObjects->at(i));
			float gl, gr, gt, gb;
			ground->GetBoundingBox(gl, gt, gr, gb);

			if (x < gr && x + HUNCHBACK_BBOX_WIDTH > gl &&
				y < gb && y + HUNCHBACK_BBOX_HEIGHT > gt &&
				vy > 0)
			{
				// Xét va chạm cứng
				SetState(HUNCHBACK_STATE_IDLE);
				isStickToGround = true;
				idleCast = GetTickCount();
			}

			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f && vy > 0)
				coEvents.push_back(e);
			else
				delete e;
		}
	}
}

void Hunchback::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case HUNCHBACK_STATE_IDLE:
		isIdle = true;
		vx = 0;
		vy = 0;
		break;
	case HUNCHBACK_STATE_JUMP:
		isIdle = false;
		vy = -(rand() % 2 + 3) * 0.1f;
		isStickToGround = false;

		vx = (rand() % 3 + 1) * 0.1f;
		if (nx < 0)
			vx = -vx;

		break;
	}
}

void Hunchback::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + HUNCHBACK_BBOX_WIDTH;
	bottom = y + HUNCHBACK_BBOX_HEIGHT;
}

void Hunchback::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (bird->isDropHunchBack && x > 0 && x > simon->x - SCREEN_WIDTH / 2 && x < simon->x + SCREEN_WIDTH / 2)
	{
		isStickToBird = false;
	}

	if (isStickToBird)
	{
		x = bird->x + BIRD_BBOX_WIDTH / 2;
		y = bird->y + BIRD_BBOX_HEIGHT;
	}
	else
	{
		Enemy::Update(dt, coObjects);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		if (isIdle)
		{
			if (GetTickCount() - idleCast > HUNCHBACK_IDLE_TIME)
			{
				if (x < simon->x)
				{
					nx = 1;
				}
				else
				{
					nx = -1;
				}

				SetState(HUNCHBACK_STATE_JUMP);
			}
		}

		if (!isStickToGround)
			vy += SIMON_GRAVITY * dt;

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Ground *>(e->obj))
				{
					// Da cham dat
					// Khong va cham theo phuong ngang
					if (e->ny < 0 || e->nx != 0)
					{
						// Xét va chạm cứng
						SetState(HUNCHBACK_STATE_IDLE);
						isStickToGround = true;
						idleCast = GetTickCount();
					}
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Hunchback::Render()
{
	int ani = -1;

	switch (state)
	{
	case HUNCHBACK_STATE_IDLE:
		if (nx > 0)
		{
			ani = HUNCHBACK_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = HUNCHBACK_ANI_IDLE_LEFT;
		}
		break;
	case HUNCHBACK_STATE_JUMP:
		if (nx > 0)
		{
			ani = HUNCHBACK_ANI_JUMP_RIGHT;
		}
		else
		{
			ani = HUNCHBACK_ANI_JUMP_LEFT;
		}
		break;
	}

	animations[ani]->Render(x, y);

	//RenderBoundingBox();
}

