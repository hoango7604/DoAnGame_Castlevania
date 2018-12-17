#include "MerMan.h"
#include "define.h"

void MerMan::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case MERMAN_STATE_JUMP:
		vy = -0.8;
		break;
	case MERMAN_STATE_WALK:
		if (nx > 0)
			vx = 0.5 * SIMON_WALKING_SPEED;
		else
			vx = -0.5 * SIMON_WALKING_SPEED;
		break;
	case MERMAN_STATE_HIT:
		vx = 0;
		isAttack = true;
		break;
	}
}

void MerMan::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	Enemy::Update(dt, coObjects);
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	// Đặt thời gian mới bắt đầu tấn công
	if (!isOnGround)
	{
		lastAttackTime = GetTickCount();
	}

	// Bật trạng thái attack
	if (isOnGround && !isAttack && GetTickCount() - lastAttackTime > MERMAN_ATTACK_DURATION_TIME)
	{
		SetState(MERMAN_STATE_HIT);
		attackTime = GetTickCount();
	}

	// Thoát trạng thái attack
	if (isAttack && GetTickCount() - attackTime > MERMAN_ATTACK_TIME)
	{
		isAttack = false;
		SetState(MERMAN_STATE_WALK);
		lastAttackTime = GetTickCount();
		didAttack = false;
	}

	// Vị trí di chuyển
	if ((x < 0 && vx < 0))
	{
		x = 0;
		vx = -vx;
		nx = -nx;
	}
	if (x > MAX_WIDTH_LV2_2 - 3 * MERMAN_BBOX_WIDTH && vx > 0)
	{
		x = MAX_WIDTH_LV2_2 - 3 * MERMAN_BBOX_WIDTH;
		vx = -vx;
		nx = -nx;
	}

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

		bool willBlock = false;

		// block

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{							
				if (e->ny < 0)
				{											
					willBlock = true;
					if (!isOnGround)
					{
						SetState(MERMAN_STATE_WALK);
						isOnGround = true;
					}
					// Xét va chạm cứng
					if (ny != 0) vy = 0;
				}
			}
		}

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
	for(int i =0;i<3;i++)
	wa_ani[i]->Update(dt,coObjects);		
	
}

void MerMan::Render()
{
	int ani = -1;

	switch (state)
	{
	case MERMAN_STATE_JUMP:
		ani = MERMAN_ANI_JUMP;
		break;
	case MERMAN_STATE_WALK:
		if (nx < 0)
		{
			ani = MERMAN_ANI_WALK_LEFT;
		}
		else if (nx > 0)
		{
			ani = MERMAN_ANI_WALK_RIGHT;
		}
		break;
	case MERMAN_STATE_HIT:
		if (nx < 0)
		{
			ani = MERMAN_ANI_HIT_LEFT;
		}
		else if (nx > 0)
		{
			ani = MERMAN_ANI_HIT_RIGHT;
		}
		break;
	}

	if (isFrozen)
		animations[ani]->ResetFrame();
	
	if (ani != -1)
		animations[ani]->Render(x, y);
	
	
	for (int i = 0; i < 3; i++)
	{
		float z, t;
		wa_ani[i]->GetPosition(z, t);
		wa_ani[i]->animations[0]->Render(z, t);
	}
	
	RenderBoundingBox();
}

void MerMan::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + MERMAN_BBOX_WIDTH;
	bottom = y + MERMAN_BBOX_HEIGHT;
}
