#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "BigFire.h"
#include "Candle.h"
#include "Zombie.h"
#include "Ground.h"
#include "Stair.h"
#include "CheckStair.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Simon::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Simon se khong va cham voi nhung vat sau:
		if (!dynamic_cast<Candle *>(coObjects->at(i)) && 
			!dynamic_cast<BigFire *>(coObjects->at(i)) && 
			!dynamic_cast<Stair *>(coObjects->at(i)) && 
			!dynamic_cast<CheckStair *>(coObjects->at(i))) 
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
		// Xet simon co dang dung ngay tai check stair hay khong
		else if (dynamic_cast<CheckStair *>(coObjects->at(i)))
		{
			CheckStair *checkstair = dynamic_cast<CheckStair *>(coObjects->at(i));
			float cbl, cbr, cbt, cbb;
			checkstair->GetBoundingBox(cbl, cbt, cbr, cbb);

			int type = checkstair->GetType();
			switch (type)
			{
			case CHECKSTAIR_UP_LEFT:
				break;
			case CHECKSTAIR_UP_RIGHT:
				if (x < cbr && x + SIMON_STAND_BBOX_WIDTH > cbl && y + SIMON_STAND_BBOX_HEIGHT == cbb)
				{
					isOnCheckStair = true;
				}
				else
				{
					isOnCheckStair = false;
				}
				break;
			case CHECKSTAIR_DOWN_LEFT:
				break;
			case CHECKSTAIR_DOWN_RIGHT:
				break;
			}
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Has completed attack animation
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isAttack = false;
		// Check collision between whip and game objects here
		whip->Update(dt, coObjects);
	}

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// Set idle state
	if (!isOnStair && !isSit && !isMoving && !isJump && !isAttack)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// when simon attack
	if (isAttack == true)
	{
		if (nx > 0)
		{
			// Whip position equal to simon position
			whip->SetPosition(x, y);
		}
		else
		{
			// Whip position equal to simon position + simon width - whip width
			whip->SetPosition(x + SIMON_STAND_BBOX_WIDTH - WHIP_BBOX_WIDTH, y);
		}
	}

	// Handle Simon go over screen camera
	float leftCorner = CGame::GetInstance()->x_cam;
	float rightCorner = leftCorner + SCREEN_WIDTH - SIMON_STAND_BBOX_WIDTH;
	// Left corner
	if (x < leftCorner)
	{
		x = 0;
	}
	// Right corner
	else if (x > rightCorner)
	{
		//x = rightCorner;
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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		/*
		 * Handle collision here
		 */
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Zombie *>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
				if (zombie->GetState() != ZOMBIE_STATE_DIE) {
					StartUntouchable();
					//SetState(SIMON_STATE_DIE);
				}
			}
			else if (dynamic_cast<Ground *>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (e->nx == 0 && e->ny < 0)
					isJump = false;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani, aniWhip = -1;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	{
		if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_HITUP_RLADDER;
					else if (ny > 0)
						ani = SIMON_ANI_HITDOWN_RLADDER;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_RIGHT;
				}
				aniWhip = WHIP_RIGHT;
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_HITUP_LLADDER;
					else if (ny > 0)
						ani = SIMON_ANI_HITDOWN_LLADDER;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_LEFT;
				}
				aniWhip = WHIP_LEFT;
			}
		}
		else if (isJump)
		{
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		// Right direction
		else if (nx > 0)
		{
			switch (state)
			{
			case SIMON_STATE_STAIR_MOVING:
				break;
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_SIT_RIGHT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_RIGHT;
				break;
			}
		}
		// Left direction
		else if (nx < 0)
		{
			switch (state)
			{
			case SIMON_STATE_STAIR_MOVING:
				break;
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				else
					ani = SIMON_ANI_SIT_LEFT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_LEFT;
				break;
			}
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[ani]->Render(x, y, alpha);

	if (aniWhip != -1)
	{
		whip->animations[aniWhip]->Render(x, y, alpha);
	}

	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_DIE:
		isDead = true;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_STAIR_MOVING:
		isOnStair = true;
		break;
	case SIMON_STATE_SIT:
		isSit = true;
		vx = 0;
		break;
	case SIMON_STATE_WALK:
		isMoving = true;
		if (nx == 1.0f)
		{
			vx = SIMON_WALKING_SPEED;
		}
		else if (nx == -1.0f)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	}
}

void Simon::SetAction(int action)
{
	switch (action)
	{
	case SIMON_ACTION_ATTACK:
		// Ngat tat ca trang thai (tru ngoi)
		// Neu dang nhay toi thi de nguyen van toc, neu khong thi dung lai va danh
		isAttack = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;
		// Don gian la cho nhay, khong ngat bat ki trang thai nao
		// Them van toc nhay
	case SIMON_ACTION_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state != SIMON_STATE_SIT)
	{
		right = x + SIMON_STAND_BBOX_WIDTH;
		bottom = y + SIMON_STAND_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_SIT_BBOX_WIDTH;
		bottom = y + SIMON_SIT_BBOX_HEIGHT;
	}

}


